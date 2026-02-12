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
#include "SidechainMeterRing.h"
#include "../../parameters/ParameterIDs.h"

class AdvancedCompressorProcessor;

class BottomControlStrip : public juce::Component
{
public:
    explicit BottomControlStrip (AdvancedCompressorProcessor& processor);
    ~BottomControlStrip() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void setGainReduction (float grDB);

    /** Callback invoked when the SC drawer toggle is clicked. */
    std::function<void()> onSidechainToggle;

    /** Sets the visual state of the SC toggle bar. */
    void setSidechainOpen (bool open);

private:
    AdvancedCompressorProcessor& processor;

    // ========== Row 1: Primary knobs (large) ==========
    juce::Slider thresholdSlider, ratioSlider, attackSlider, releaseSlider, outputGainSlider;
    juce::Label thresholdLabel, ratioLabel, attackLabel, releaseLabel, outputGainLabel;
    SidechainMeterRing thresholdRing;

    // Style selector (centered)
    juce::ComboBox styleSelector;
    juce::Label styleLabel;

    // Row 1 buttons
    juce::TextButton analyzerButton { "Analyzer" };
    juce::TextButton autoGainButton { "Auto Gain" };

    // ========== Row 2: Secondary controls (compact) ==========
    // Group A: Dynamics refinement
    juce::Slider kneeSlider, rangeSlider, holdSlider, lookaheadSlider;
    juce::Label kneeLabel, rangeLabel, holdLabel, lookaheadLabel;

    // Group B: Output/stereo
    juce::Slider mixSlider, wetGainSlider, stereoLinkSlider;
    juce::ComboBox stereoModeSelector;
    juce::Label mixLabel, wetGainLabel, stereoLinkLabel, stereoModeLabel;

    // Group C: Input/character
    juce::Slider inputGainSlider, charDriveSlider;
    juce::ComboBox charModeSelector;
    juce::Label inputGainLabel, charDriveLabel, charModeLabel;

    // Row 2 small buttons/combos
    juce::ComboBox oversampleSelector;
    juce::ComboBox meterScaleSelector;
    juce::TextButton autoThreshButton { "Auto Thr" };
    juce::TextButton midiTriggerButton { "MIDI Trg" };
    juce::TextButton analyzerPreButton { "Pre" };
    juce::Label oversampleLabel, meterScaleLabel;

    // ========== SC toggle bar ==========
    juce::TextButton scToggleButton { "SIDE CHAIN" };
    bool scDrawerOpen = false;

    // ========== APVTS Attachments ==========
    // Row 1
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> styleAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> analyzerAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoGainAttach;

    // Row 2
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kneeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> holdAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lookaheadAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stereoLinkAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> stereoModeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> charDriveAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> charModeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oversampleAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> meterScaleAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoThreshAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midiTriggerAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> analyzerPreAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomControlStrip)
};
