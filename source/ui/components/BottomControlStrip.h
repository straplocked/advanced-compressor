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
#include "SidechainPanel.h"
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

private:
    AdvancedCompressorProcessor& processor;

    // ========== Tab bar ==========
    juce::TextButton basicTab { "Basic" };
    juce::TextButton advancedTab { "Advanced" };
    juce::TextButton sidechainTab { "Sidechain" };
    int activeTab = 0;
    void setActiveTab (int tab);

    // ========== Tab containers ==========
    juce::Component basicPanel;
    juce::Component advancedPanel;
    juce::Component sidechainContent;
    SidechainPanel sidechainPanel;

    // ========== Basic tab: Primary knobs (large) ==========
    juce::Slider thresholdSlider, ratioSlider, attackSlider, releaseSlider, mixSlider, outputGainSlider;
    juce::Label thresholdTitle, ratioTitle, attackTitle, releaseTitle, mixTitle, outputGainTitle;
    juce::Label thresholdValue, ratioValue, attackValue, releaseValue, mixValue, outputGainValue;
    SidechainMeterRing thresholdRing;

    // Style selector (centered)
    juce::ComboBox styleSelector;
    juce::Label styleLabel;

    // Basic tab buttons
    juce::TextButton analyzerButton { "Analyzer" };
    juce::TextButton autoGainButton { "Auto Gain" };

    // ========== Advanced tab: Secondary controls (compact) ==========
    // Group A: Dynamics refinement
    juce::Slider kneeSlider, rangeSlider, holdSlider, lookaheadSlider;
    juce::Label kneeTitle, rangeTitle, holdTitle, lookaheadTitle;
    juce::Label kneeValue, rangeValue, holdValue, lookaheadValue;

    // Group B: Output/stereo
    juce::Slider wetGainSlider, stereoLinkSlider;
    juce::ComboBox stereoModeSelector;
    juce::Label wetGainTitle, stereoLinkTitle, stereoModeLabel;
    juce::Label wetGainValue, stereoLinkValue;

    // Group C: Input/character
    juce::Slider inputGainSlider, charDriveSlider;
    juce::ComboBox charModeSelector;
    juce::Label inputGainTitle, charDriveTitle, charModeLabel;
    juce::Label inputGainValue, charDriveValue;

    // Utility strip
    juce::ComboBox oversampleSelector;
    juce::ComboBox meterScaleSelector;
    juce::TextButton autoThreshButton { "Auto Thr" };
    juce::TextButton midiTriggerButton { "MIDI Trg" };
    juce::TextButton analyzerPreButton { "Pre" };
    juce::Label oversampleLabel, meterScaleLabel;

    // ========== APVTS Attachments ==========
    // Basic tab
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> styleAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> analyzerAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoGainAttach;

    // Advanced tab
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kneeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> holdAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lookaheadAttach;
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

    void updateValueLabels();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomControlStrip)
};
