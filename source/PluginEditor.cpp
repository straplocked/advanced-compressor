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
      controlStrip (p),
      presetBrowser (p.getPresetManager())
{
    setLookAndFeel (&lookAndFeel);

    addAndMakeVisible (headerToolbar);
    addAndMakeVisible (compressorDisplay);
    addAndMakeVisible (controlStrip);
    addChildComponent (presetBrowser);  // hidden initially
    addAndMakeVisible (helpOverlay);

    // Preset name click - toggle browser panel
    headerToolbar.onPresetNameClicked = [this]
    {
        presetBrowser.setVisible (! presetBrowser.isVisible());
    };

    setSize (Constants::kDefaultEditorWidth, Constants::kDefaultEditorHeight);
    setResizable (true, true);
    setResizeLimits (860, 495, 1800, 1350);
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

    // Bottom panel: tabbed control strip (28 tab bar + 142 content + 30 utility)
    int bottomPanelH = 200;
    controlStrip.setBounds (bounds.removeFromBottom (bottomPanelH));

    compressorDisplay.setBounds (bounds);

    // Preset browser panel (right overlay on compressor display)
    auto displayBounds = compressorDisplay.getBounds();
    presetBrowser.setBounds (displayBounds.getRight() - 350,
                              displayBounds.getY(),
                              350, displayBounds.getHeight());

    // Help overlay covers everything
    helpOverlay.setBounds (getLocalBounds());
    helpOverlay.toFront (false);
}
