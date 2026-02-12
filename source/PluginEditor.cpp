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
      sidechainPanel (p.getAPVTS())
{
    setLookAndFeel (&lookAndFeel);

    addAndMakeVisible (headerToolbar);
    addAndMakeVisible (compressorDisplay);
    addAndMakeVisible (controlStrip);
    addChildComponent (sidechainPanel); // hidden initially
    addAndMakeVisible (helpOverlay);

    // Wire SC drawer toggle
    controlStrip.onSidechainToggle = [this] { toggleSidechainDrawer(); };

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

    headerToolbar.setBounds (bounds.removeFromTop (40));

    // Bottom panel: control strip (Row1 + Row2 + SC bar) = ~204px
    int bottomPanelH = 204;
    int scDrawerH = scDrawerOpen ? 140 : 0;

    // SC drawer at the very bottom (when open)
    if (scDrawerOpen)
    {
        sidechainPanel.setBounds (bounds.removeFromBottom (scDrawerH));
    }

    controlStrip.setBounds (bounds.removeFromBottom (bottomPanelH));
    compressorDisplay.setBounds (bounds);

    // Help overlay covers everything
    helpOverlay.setBounds (getLocalBounds());
    helpOverlay.toFront (false);
}

void AdvancedCompressorEditor::toggleSidechainDrawer()
{
    scDrawerOpen = ! scDrawerOpen;
    controlStrip.setSidechainOpen (scDrawerOpen);

    if (scDrawerOpen)
    {
        sidechainPanel.setVisible (true);
    }

    // Animate the resize
    int newHeight = scDrawerOpen
        ? getHeight() + 140
        : getHeight() - 140;

    // Clamp to limits
    auto [minW, minH, maxW, maxH] = std::tuple { 860, 495, 1800, 1350 };
    newHeight = juce::jlimit (minH, maxH, newHeight);

    // Use animator for smooth transition
    auto targetBounds = getBounds().withHeight (newHeight);
    animator.animateComponent (this, targetBounds, 1.0f, 200, false, 0.5, 0.0);

    if (! scDrawerOpen)
    {
        // Hide after animation would complete
        juce::Timer::callAfterDelay (220, [this]
        {
            if (! scDrawerOpen)
                sidechainPanel.setVisible (false);
        });
    }
}
