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
#include "../LookAndFeel/ColourPalette.h"

/**
 * @brief A button that paints a juce::Path icon with hover/active states.
 *
 * The icon path should be normalized to a 0-1 bounding box.
 * All icons are drawn as stroked outlines for consistent appearance.
 */
class IconButton : public juce::Button
{
public:
    IconButton (const juce::String& name, juce::Path iconPath, float strokeThickness = 1.6f)
        : juce::Button (name),
          icon (std::move (iconPath)),
          stroke (strokeThickness)
    {
    }

    void setIconPath (juce::Path newPath)
    {
        icon = std::move (newPath);
        repaint();
    }

    void paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (4.0f);
        float size = juce::jmin (bounds.getWidth(), bounds.getHeight());

        auto scaledIcon = icon;
        scaledIcon.applyTransform (
            juce::AffineTransform::scale (size, size)
                .translated (bounds.getCentreX() - size * 0.5f,
                             bounds.getCentreY() - size * 0.5f));

        juce::Colour colour;
        if (getToggleState())
            colour = activeColour;
        else if (isButtonDown)
            colour = ColourPalette::textBright();
        else if (isMouseOver)
            colour = ColourPalette::textBright().withAlpha (0.8f);
        else
            colour = ColourPalette::textDim();

        if (getToggleState() && isMouseOver)
            colour = activeColour.brighter (0.2f);

        // Tinted background on toggle-on state
        if (getToggleState())
        {
            g.setColour (activeColour.withAlpha (0.15f));
            g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (1.0f), 3.0f);
        }

        // Draw all icons as stroked outlines
        g.setColour (colour);
        g.strokePath (scaledIcon, juce::PathStrokeType (
            stroke, juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));
    }

    void setActiveColour (juce::Colour c) { activeColour = c; }

private:
    juce::Path icon;
    float stroke;
    juce::Colour activeColour { ColourPalette::thresholdAccent() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconButton)
};
