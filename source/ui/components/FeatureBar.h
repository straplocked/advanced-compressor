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
#include "../LookAndFeel/ColourPalette.h"
#include "../../parameters/ParameterIDs.h"

class AdvancedCompressorProcessor;

class FeatureBar : public juce::Component
{
public:
    explicit FeatureBar (AdvancedCompressorProcessor& processor);
    ~FeatureBar() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    AdvancedCompressorProcessor& processor;

    // Compact knobs
    juce::Slider inputGainSlider;
    juce::Slider lookaheadSlider;
    juce::Slider charDriveSlider;

    // Toggle buttons
    juce::TextButton autoGainButton { "Auto Gain" };
    juce::TextButton autoThreshButton { "Auto Thr" };
    juce::TextButton analyzerPreButton { "Pre" };
    juce::TextButton midiTriggerButton { "MIDI Trg" };

    // Combo boxes
    juce::ComboBox charModeSelector;
    juce::ComboBox meterScaleSelector;

    // Labels
    juce::Label inputGainLabel, lookaheadLabel, charDriveLabel;
    juce::Label charModeLabel, meterScaleLabel;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lookaheadAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> charDriveAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoThreshAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> analyzerPreAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midiTriggerAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> charModeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> meterScaleAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeatureBar)
};
