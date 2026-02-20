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
#include "KneeDisplay.h"
#include "LevelMeterComponent.h"
#include "GainReductionDisplay.h"
#include "GRHistoryGraph.h"
#include "../../analyzer/SpectrumAnalyzer.h"
#include "../../analyzer/FFTProcessor.h"
#include "../../metering/LevelMeterData.h"
#include "../../metering/PeakHoldCalculator.h"
#include "../LookAndFeel/ColourPalette.h"
#include "../../utils/Constants.h"

// Forward declaration
class AdvancedCompressorProcessor;

class CompressorDisplay : public juce::Component,
                           public juce::Timer
{
public:
    explicit CompressorDisplay (AdvancedCompressorProcessor& processor);
    ~CompressorDisplay() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    AdvancedCompressorProcessor& processor;

    KneeDisplay kneeDisplay;
    LevelMeterComponent inputMeter;
    LevelMeterComponent outputMeter;
    GainReductionDisplay grDisplay;
    GRHistoryGraph grHistory;

    SpectrumAnalyzer spectrumPre;
    SpectrumAnalyzer spectrumPost;

    PeakHoldCalculator inputPeakL, inputPeakR;
    PeakHoldCalculator outputPeakL, outputPeakR;

    // Loudness readout
    juce::Label loudnessLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorDisplay)
};
