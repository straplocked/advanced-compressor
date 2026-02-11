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

#include "PluginEditor.h"
#include "utils/Constants.h"
#include "ui/LookAndFeel/ColourPalette.h"

AdvancedCompressorEditor::AdvancedCompressorEditor (AdvancedCompressorProcessor& p)
    : AudioProcessorEditor (&p),
      headerToolbar (p),
      compressorDisplay (p),
      controlStrip (p)
{
    setLookAndFeel (&lookAndFeel);

    addAndMakeVisible (headerToolbar);
    addAndMakeVisible (compressorDisplay);
    addAndMakeVisible (controlStrip);

    setSize (Constants::kDefaultEditorWidth, Constants::kDefaultEditorHeight);
    setResizable (true, true);
    setResizeLimits (750, 450, 1920, 1080);
}

AdvancedCompressorEditor::~AdvancedCompressorEditor()
{
    setLookAndFeel (nullptr);
}

void AdvancedCompressorEditor::paint (juce::Graphics& g)
{
    g.fillAll (ColourPalette::background());
}

void AdvancedCompressorEditor::resized()
{
    auto bounds = getLocalBounds();

    headerToolbar.setBounds (bounds.removeFromTop (36));
    controlStrip.setBounds (bounds.removeFromBottom (110));
    compressorDisplay.setBounds (bounds);
}
