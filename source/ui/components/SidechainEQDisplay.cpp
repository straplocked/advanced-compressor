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

#include "SidechainEQDisplay.h"
#include "../../parameters/ParameterIDs.h"
#include "../../utils/Constants.h"
#include "../../utils/FrequencyUtils.h"
#include "../LookAndFeel/ColourPalette.h"
#include "../LookAndFeel/Fonts.h"
#include "../../dsp/FilterTypes.h"

// Butterworth Q values — must mirror SidechainEQ exactly
static constexpr double kButterworthQ_Order2[]  = { 0.7071 };
static constexpr double kButterworthQ_Order4[]  = { 0.5412, 1.3066 };
static constexpr double kButterworthQ_Order8[]  = { 0.5098, 0.6013, 0.8999, 2.5629 };
static constexpr double kButterworthQ_Order16[] = { 0.5024, 0.5224, 0.5669, 0.6468,
                                                     0.7845, 1.0353, 1.6056, 5.1258 };

// All sidechain parameter IDs that the display listens to
static const juce::String kSCParamIDs[] = {
    ParamIDs::kSCLCFreq, ParamIDs::kSCLCOn, ParamIDs::kSCLCSlope,
    ParamIDs::kSCLSFreq, ParamIDs::kSCLSGain, ParamIDs::kSCLSOn,
    ParamIDs::kSCBandFreq, ParamIDs::kSCBandGain, ParamIDs::kSCBandQ,
    ParamIDs::kSCBand2Freq, ParamIDs::kSCBand2Gain, ParamIDs::kSCBand2Q,
    ParamIDs::kSCHSFreq, ParamIDs::kSCHSGain, ParamIDs::kSCHSOn,
    ParamIDs::kSCHCFreq, ParamIDs::kSCHCOn, ParamIDs::kSCHCSlope
};

SidechainEQDisplay::SidechainEQDisplay (juce::AudioProcessorValueTreeState& apvtsRef)
    : apvts (apvtsRef)
{
    setInterceptsMouseClicks (false, false);

    // Prepare all UI-side filters at the display sample rate
    for (auto& f : lcFilters) f.prepare (kDisplaySampleRate);
    for (auto& f : hcFilters) f.prepare (kDisplaySampleRate);
    lsFilter.prepare (kDisplaySampleRate);
    band1Filter.prepare (kDisplaySampleRate);
    band2Filter.prepare (kDisplaySampleRate);
    hsFilter.prepare (kDisplaySampleRate);

    // Listen for all SC parameter changes
    for (const auto& id : kSCParamIDs)
        apvts.addParameterListener (id, this);

    startTimerHz (Constants::kUITimerHz);
}

SidechainEQDisplay::~SidechainEQDisplay()
{
    stopTimer();

    for (const auto& id : kSCParamIDs)
        apvts.removeParameterListener (id, this);
}

void SidechainEQDisplay::parameterChanged (const juce::String&, float)
{
    dirty.store (true, std::memory_order_relaxed);
}

void SidechainEQDisplay::timerCallback()
{
    if (dirty.exchange (false, std::memory_order_relaxed))
    {
        rebuildCurves();
        repaint();
    }
}

void SidechainEQDisplay::configureCascade (SVFFilter* sections, int& numActive,
                                            FilterType type, float freq, int slopeIndex)
{
    switch (slopeIndex)
    {
        case 0: // 6 dB/oct — single first-order section
            numActive = 1;
            sections[0].setFirstOrder (true);
            sections[0].setParameters (type, freq, 0.7071, 0.0);
            break;

        case 1: // 12 dB/oct — single second-order section
            numActive = 1;
            sections[0].setFirstOrder (false);
            sections[0].setParameters (type, freq, kButterworthQ_Order2[0], 0.0);
            break;

        case 2: // 24 dB/oct — 2 sections
            numActive = 2;
            for (int i = 0; i < 2; ++i)
            {
                sections[i].setFirstOrder (false);
                sections[i].setParameters (type, freq, kButterworthQ_Order4[i], 0.0);
            }
            break;

        case 3: // 48 dB/oct — 4 sections
            numActive = 4;
            for (int i = 0; i < 4; ++i)
            {
                sections[i].setFirstOrder (false);
                sections[i].setParameters (type, freq, kButterworthQ_Order8[i], 0.0);
            }
            break;

        case 4: // 96 dB/oct — 8 sections
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

void SidechainEQDisplay::rebuildCurves()
{
    if (displayWidth <= 0 || displayHeight <= 0)
        return;

    // Read all sidechain parameters from APVTS
    auto* lcFreqParam  = apvts.getRawParameterValue (ParamIDs::kSCLCFreq);
    auto* lcOnParam    = apvts.getRawParameterValue (ParamIDs::kSCLCOn);
    auto* lcSlopeParam = apvts.getRawParameterValue (ParamIDs::kSCLCSlope);
    auto* lsFreqParam  = apvts.getRawParameterValue (ParamIDs::kSCLSFreq);
    auto* lsGainParam  = apvts.getRawParameterValue (ParamIDs::kSCLSGain);
    auto* lsOnParam    = apvts.getRawParameterValue (ParamIDs::kSCLSOn);
    auto* b1FreqParam  = apvts.getRawParameterValue (ParamIDs::kSCBandFreq);
    auto* b1GainParam  = apvts.getRawParameterValue (ParamIDs::kSCBandGain);
    auto* b1QParam     = apvts.getRawParameterValue (ParamIDs::kSCBandQ);
    auto* b2FreqParam  = apvts.getRawParameterValue (ParamIDs::kSCBand2Freq);
    auto* b2GainParam  = apvts.getRawParameterValue (ParamIDs::kSCBand2Gain);
    auto* b2QParam     = apvts.getRawParameterValue (ParamIDs::kSCBand2Q);
    auto* hsFreqParam  = apvts.getRawParameterValue (ParamIDs::kSCHSFreq);
    auto* hsGainParam  = apvts.getRawParameterValue (ParamIDs::kSCHSGain);
    auto* hsOnParam    = apvts.getRawParameterValue (ParamIDs::kSCHSOn);
    auto* hcFreqParam  = apvts.getRawParameterValue (ParamIDs::kSCHCFreq);
    auto* hcOnParam    = apvts.getRawParameterValue (ParamIDs::kSCHCOn);
    auto* hcSlopeParam = apvts.getRawParameterValue (ParamIDs::kSCHCSlope);

    bool  lcOn      = lcOnParam->load() >= 0.5f;
    float lcFreq    = lcFreqParam->load();
    int   lcSlope   = (int) lcSlopeParam->load();
    bool  lsOn      = lsOnParam->load() >= 0.5f;
    float lsFreq    = lsFreqParam->load();
    float lsGain    = lsGainParam->load();
    float b1Freq    = b1FreqParam->load();
    float b1Gain    = b1GainParam->load();
    float b1Q       = b1QParam->load();
    float b2Freq    = b2FreqParam->load();
    float b2Gain    = b2GainParam->load();
    float b2Q       = b2QParam->load();
    bool  hsOn      = hsOnParam->load() >= 0.5f;
    float hsFreq    = hsFreqParam->load();
    float hsGain    = hsGainParam->load();
    bool  hcOn      = hcOnParam->load() >= 0.5f;
    float hcFreq    = hcFreqParam->load();
    int   hcSlope   = (int) hcSlopeParam->load();

    // Active state mirrors SidechainEQ processing logic
    bool lcActive = lcOn;
    bool lsActive = lsOn && std::abs (lsGain) > 0.01f;
    bool b1Active = std::abs (b1Gain) > 0.01f;
    bool b2Active = std::abs (b2Gain) > 0.01f;
    bool hsActive = hsOn && std::abs (hsGain) > 0.01f;
    bool hcActive = hcOn;

    // Configure UI-side filters
    if (lcActive) configureCascade (lcFilters.data(), numLCActive, FilterType::HighPass, lcFreq, lcSlope);
    if (lsActive) lsFilter.setParameters (FilterType::LowShelf, lsFreq, 0.7071, lsGain);
    if (b1Active) band1Filter.setParameters (FilterType::Bell, b1Freq, b1Q, b1Gain);
    if (b2Active) band2Filter.setParameters (FilterType::Bell, b2Freq, b2Q, b2Gain);
    if (hsActive) hsFilter.setParameters (FilterType::HighShelf, hsFreq, 0.7071, hsGain);
    if (hcActive) configureCascade (hcFilters.data(), numHCActive, FilterType::LowPass, hcFreq, hcSlope);

    bool bandActive[] = { lcActive, lsActive, b1Active, b2Active, hsActive, hcActive };

    // Build paths
    totalCurvePath.clear();
    totalFillPath.clear();
    for (auto& p : bandPaths)
        p.clear();

    float logMin = std::log (kFreqMin);
    float logMax = std::log (kFreqMax);

    for (int i = 0; i <= kNumPoints; ++i)
    {
        float t = (float) i / (float) kNumPoints;
        float freq = std::exp (logMin + t * (logMax - logMin));
        float x = t * displayWidth;

        // Per-band dB contributions
        double bandDB[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

        if (lcActive)
            for (int s = 0; s < numLCActive; ++s)
                bandDB[0] += lcFilters[(size_t) s].getMagnitudeAtFrequency (freq);

        if (lsActive) bandDB[1] = lsFilter.getMagnitudeAtFrequency (freq);
        if (b1Active) bandDB[2] = band1Filter.getMagnitudeAtFrequency (freq);
        if (b2Active) bandDB[3] = band2Filter.getMagnitudeAtFrequency (freq);
        if (hsActive) bandDB[4] = hsFilter.getMagnitudeAtFrequency (freq);

        if (hcActive)
            for (int s = 0; s < numHCActive; ++s)
                bandDB[5] += hcFilters[(size_t) s].getMagnitudeAtFrequency (freq);

        double totalDB = 0.0;
        for (int b = 0; b < 6; ++b)
            totalDB += bandDB[b];

        float y = dbToY ((float) totalDB);

        if (i == 0)
        {
            totalCurvePath.startNewSubPath (x, y);
            totalFillPath.startNewSubPath (x, displayHeight * 0.5f);
            totalFillPath.lineTo (x, y);
        }
        else
        {
            totalCurvePath.lineTo (x, y);
            totalFillPath.lineTo (x, y);
        }

        // Per-band paths
        for (int b = 0; b < 6; ++b)
        {
            if (bandActive[b])
            {
                float by = dbToY ((float) bandDB[b]);
                if (i == 0) bandPaths[(size_t) b].startNewSubPath (x, by);
                else        bandPaths[(size_t) b].lineTo (x, by);
            }
        }
    }

    // Close fill path
    totalFillPath.lineTo (displayWidth, displayHeight * 0.5f);
    totalFillPath.closeSubPath();
}

float SidechainEQDisplay::freqToX (float freq) const
{
    return displayWidth * std::log (freq / kFreqMin) / std::log (kFreqMax / kFreqMin);
}

float SidechainEQDisplay::dbToY (float dB) const
{
    float clamped = juce::jlimit (-kDBRange, kDBRange, dB);
    return displayHeight * (0.5f - clamped / (2.0f * kDBRange));
}

void SidechainEQDisplay::resized()
{
    displayWidth = (float) getWidth();
    displayHeight = (float) getHeight();
    dirty.store (true, std::memory_order_relaxed);
}

void SidechainEQDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background fill (dark curve area)
    g.setColour (ColourPalette::curveArea().withAlpha (0.5f));
    g.fillRoundedRectangle (bounds, 4.0f);

    // Grid: 0 dB center line
    float zeroY = displayHeight * 0.5f;
    g.setColour (ColourPalette::gridLineStrong().withAlpha (0.5f));
    g.drawHorizontalLine ((int) zeroY, 0.0f, displayWidth);

    // Grid: +/-6, +/-12 dB lines
    for (float dB : { -12.0f, -6.0f, 6.0f, 12.0f })
    {
        float y = dbToY (dB);
        g.setColour (ColourPalette::gridLine().withAlpha (0.3f));
        g.drawHorizontalLine ((int) y, 0.0f, displayWidth);
    }

    // Frequency grid lines at decade marks
    for (float freq : { 50.0f, 100.0f, 200.0f, 500.0f, 1000.0f,
                         2000.0f, 5000.0f, 10000.0f })
    {
        float x = freqToX (freq);
        g.setColour (ColourPalette::gridLine().withAlpha (0.25f));
        g.drawVerticalLine ((int) x, 0.0f, displayHeight);
    }

    // Per-band curves with unique colours
    juce::Colour bandColours[] = {
        ColourPalette::scBandLC(), ColourPalette::scBandLS(),
        ColourPalette::scBandB1(), ColourPalette::scBandB2(),
        ColourPalette::scBandHS(), ColourPalette::scBandHC()
    };
    for (int b = 0; b < 6; ++b)
    {
        if (! bandPaths[(size_t) b].isEmpty())
        {
            g.setColour (bandColours[b].withAlpha (0.2f));
            g.strokePath (bandPaths[(size_t) b], juce::PathStrokeType (1.0f));
        }
    }

    // Summed response fill
    if (! totalFillPath.isEmpty())
    {
        g.setColour (ColourPalette::spectrumPost().withAlpha (0.15f));
        g.fillPath (totalFillPath);
    }

    // Summed response curve stroke
    if (! totalCurvePath.isEmpty())
    {
        g.setColour (ColourPalette::transferCurve().withAlpha (0.8f));
        g.strokePath (totalCurvePath, juce::PathStrokeType (1.5f));
    }

    // Bottom-edge frequency labels
    g.setFont (Fonts::getBody (9.0f));
    g.setColour (ColourPalette::axisText().withAlpha (0.5f));
    for (float freq : { 100.0f, 1000.0f, 10000.0f })
    {
        float x = freqToX (freq);
        g.drawText (FrequencyUtils::formatFrequency (freq),
                    (int) x - 20, (int) displayHeight - 12, 40, 12,
                    juce::Justification::centred);
    }
}
