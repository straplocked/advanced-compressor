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
#include "../../presets/PresetManager.h"
#include "../LookAndFeel/ColourPalette.h"
#include "IconButton.h"

class AdvancedCompressorProcessor;

class HeaderToolbar : public juce::Component,
                      public PresetManager::Listener
{
public:
    explicit HeaderToolbar (AdvancedCompressorProcessor& processor);
    ~HeaderToolbar() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void presetChanged() override;

private:
    AdvancedCompressorProcessor& processor;
    PresetManager& presetManager;

    // Preset controls
    IconButton prevButton;
    IconButton nextButton;
    juce::TextButton presetNameButton { "Init" };
    IconButton saveButton;

    // Actions
    IconButton undoButton;
    IconButton redoButton;
    IconButton deltaButton;

    // A/B comparison (kept as text)
    juce::TextButton abButton { "A" };
    juce::TextButton copyABButton { "A>B" };
    juce::TextButton midiLearnButton { "MIDI" };

    // APVTS attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> deltaAttachment;

    void showPresetMenu();
    void showSaveDialog();
    void updatePresetName();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderToolbar)
};
