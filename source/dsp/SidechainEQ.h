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

#pragma once

#include "SVFFilter.h"
#include <array>
#include <cmath>

class SidechainEQ
{
public:
    static constexpr int kMaxCascadeSections = 8; // 96 dB/oct = order 16 = 8 biquads

    SidechainEQ() = default;

    void prepare (double sampleRate);
    void reset();

    // Band 1: Low Cut (HP) with variable slope
    void setLowCut (float freq, bool enabled, int slopeIndex);

    // Band 2: Low Shelf
    void setLowShelf (float freq, float gainDB, bool enabled);

    // Band 3: Bell 1
    void setBand1 (float freq, float gainDB, float q);

    // Band 4: Bell 2
    void setBand2 (float freq, float gainDB, float q);

    // Band 5: High Shelf
    void setHighShelf (float freq, float gainDB, bool enabled);

    // Band 6: High Cut (LP) with variable slope
    void setHighCut (float freq, bool enabled, int slopeIndex);

    float processSample (float input);
    void process (float* buffer, int numSamples);

private:
    void configureCascade (SVFFilter* sections, int& numActive,
                           FilterType type, float freq, int slopeIndex);

    // Butterworth Q values for cascaded second-order sections
    static constexpr double kButterworthQ_Order2[] = { 0.7071 };
    static constexpr double kButterworthQ_Order4[] = { 0.5412, 1.3066 };
    static constexpr double kButterworthQ_Order8[] = { 0.5098, 0.6013, 0.8999, 2.5629 };
    static constexpr double kButterworthQ_Order16[] = { 0.5024, 0.5224, 0.5669, 0.6468,
                                                         0.7845, 1.0353, 1.6056, 5.1258 };

    // Low Cut cascade
    std::array<SVFFilter, kMaxCascadeSections> lcSections;
    int numLCActive = 0;
    bool lcEnabled = false;

    // Low Shelf
    SVFFilter lowShelfFilter;
    bool lsEnabled = false;
    float lsGain = 0.0f;

    // Band 1 (Bell)
    SVFFilter band1Filter;
    float band1Gain = 0.0f;

    // Band 2 (Bell)
    SVFFilter band2Filter;
    float band2Gain = 0.0f;

    // High Shelf
    SVFFilter highShelfFilter;
    bool hsEnabled = false;
    float hsGain = 0.0f;

    // High Cut cascade
    std::array<SVFFilter, kMaxCascadeSections> hcSections;
    int numHCActive = 0;
    bool hcEnabled = false;

    double currentSampleRate = 44100.0;
};
