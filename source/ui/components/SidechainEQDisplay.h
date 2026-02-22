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

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <array>
#include <atomic>
#include "../../dsp/SVFFilter.h"
#include "../../dsp/SidechainEQ.h"

class SidechainEQDisplay : public juce::Component,
                            public juce::Timer,
                            private juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit SidechainEQDisplay (juce::AudioProcessorValueTreeState& apvts);
    ~SidechainEQDisplay() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    void parameterChanged (const juce::String& parameterID, float newValue) override;

private:
    void rebuildCurves();
    void configureCascade (SVFFilter* sections, int& numActive,
                            FilterType type, float freq, int slopeIndex);

    float freqToX (float freq) const;
    float dbToY (float dB) const;

    juce::AudioProcessorValueTreeState& apvts;

    // UI-thread-only filter instances for response computation
    std::array<SVFFilter, SidechainEQ::kMaxCascadeSections> lcFilters;
    std::array<SVFFilter, SidechainEQ::kMaxCascadeSections> hcFilters;
    SVFFilter lsFilter, band1Filter, band2Filter, hsFilter;
    int numLCActive = 0, numHCActive = 0;

    // Cached paths
    juce::Path totalCurvePath;
    juce::Path totalFillPath;
    std::array<juce::Path, 6> bandPaths;

    // Display state
    std::atomic<bool> dirty { true };
    float displayWidth = 0.0f;
    float displayHeight = 0.0f;

    static constexpr float kFreqMin = 20.0f;
    static constexpr float kFreqMax = 20000.0f;
    static constexpr float kDBRange = 30.0f;
    static constexpr int kNumPoints = 512;
    static constexpr double kDisplaySampleRate = 48000.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidechainEQDisplay)
};
