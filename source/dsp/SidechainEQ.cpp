/*
 * Advanced Compressor - Open-source dynamic range compressor plugin
 * Copyright (C) 2024-2026 Advanced Compressor contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "SidechainEQ.h"
#include "FilterTypes.h"

void SidechainEQ::prepare (double sampleRate)
{
    currentSampleRate = sampleRate;

    for (auto& s : lcSections)
        s.prepare (sampleRate);
    lowShelfFilter.prepare (sampleRate);
    band1Filter.prepare (sampleRate);
    band2Filter.prepare (sampleRate);
    highShelfFilter.prepare (sampleRate);
    for (auto& s : hcSections)
        s.prepare (sampleRate);
}

void SidechainEQ::reset()
{
    for (auto& s : lcSections)
        s.reset();
    lowShelfFilter.reset();
    band1Filter.reset();
    band2Filter.reset();
    highShelfFilter.reset();
    for (auto& s : hcSections)
        s.reset();
}

void SidechainEQ::configureCascade (SVFFilter* sections, int& numActive,
                                     FilterType type, float freq, int slopeIndex)
{
    // slopeIndex: 0=6dB/oct, 1=12dB/oct, 2=24dB/oct, 3=48dB/oct, 4=96dB/oct
    switch (slopeIndex)
    {
        case 0: // 6 dB/oct -- single first-order section
            numActive = 1;
            sections[0].setFirstOrder (true);
            sections[0].setParameters (type, freq, 0.7071, 0.0);
            break;

        case 1: // 12 dB/oct -- single second-order section
            numActive = 1;
            sections[0].setFirstOrder (false);
            sections[0].setParameters (type, freq, kButterworthQ_Order2[0], 0.0);
            break;

        case 2: // 24 dB/oct -- 2 sections
            numActive = 2;
            for (int i = 0; i < 2; ++i)
            {
                sections[i].setFirstOrder (false);
                sections[i].setParameters (type, freq, kButterworthQ_Order4[i], 0.0);
            }
            break;

        case 3: // 48 dB/oct -- 4 sections
            numActive = 4;
            for (int i = 0; i < 4; ++i)
            {
                sections[i].setFirstOrder (false);
                sections[i].setParameters (type, freq, kButterworthQ_Order8[i], 0.0);
            }
            break;

        case 4: // 96 dB/oct -- 8 sections
            numActive = 8;
            for (int i = 0; i < 8; ++i)
            {
                sections[i].setFirstOrder (false);
                sections[i].setParameters (type, freq, kButterworthQ_Order16[i], 0.0);
            }
            break;

        default:
            numActive = 1;
            sections[0].setFirstOrder (false);
            sections[0].setParameters (type, freq, 0.7071, 0.0);
            break;
    }
}

void SidechainEQ::setLowCut (float freq, bool enabled, int slopeIndex)
{
    lcEnabled = enabled;
    if (enabled)
        configureCascade (lcSections.data(), numLCActive, FilterType::HighPass, freq, slopeIndex);
}

void SidechainEQ::setLowShelf (float freq, float gainDB, bool enabled)
{
    lsEnabled = enabled;
    lsGain = gainDB;
    if (enabled && std::abs (gainDB) > 0.01f)
        lowShelfFilter.setParameters (FilterType::LowShelf, freq, 0.7071, gainDB);
}

void SidechainEQ::setBand1 (float freq, float gainDB, float q)
{
    band1Gain = gainDB;
    band1Filter.setParameters (FilterType::Bell, freq, q, gainDB);
}

void SidechainEQ::setBand2 (float freq, float gainDB, float q)
{
    band2Gain = gainDB;
    band2Filter.setParameters (FilterType::Bell, freq, q, gainDB);
}

void SidechainEQ::setHighShelf (float freq, float gainDB, bool enabled)
{
    hsEnabled = enabled;
    hsGain = gainDB;
    if (enabled && std::abs (gainDB) > 0.01f)
        highShelfFilter.setParameters (FilterType::HighShelf, freq, 0.7071, gainDB);
}

void SidechainEQ::setHighCut (float freq, bool enabled, int slopeIndex)
{
    hcEnabled = enabled;
    if (enabled)
        configureCascade (hcSections.data(), numHCActive, FilterType::LowPass, freq, slopeIndex);
}

float SidechainEQ::processSample (float input)
{
    float out = input;

    // Band 1: Low Cut
    if (lcEnabled)
    {
        for (int i = 0; i < numLCActive; ++i)
            out = lcSections[(size_t) i].processSample (out);
    }

    // Band 2: Low Shelf
    if (lsEnabled && std::abs (lsGain) > 0.01f)
        out = lowShelfFilter.processSample (out);

    // Band 3: Bell 1
    if (std::abs (band1Gain) > 0.01f)
        out = band1Filter.processSample (out);

    // Band 4: Bell 2
    if (std::abs (band2Gain) > 0.01f)
        out = band2Filter.processSample (out);

    // Band 5: High Shelf
    if (hsEnabled && std::abs (hsGain) > 0.01f)
        out = highShelfFilter.processSample (out);

    // Band 6: High Cut
    if (hcEnabled)
    {
        for (int i = 0; i < numHCActive; ++i)
            out = hcSections[(size_t) i].processSample (out);
    }

    return out;
}

void SidechainEQ::process (float* buffer, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        buffer[i] = processSample (buffer[i]);
}
