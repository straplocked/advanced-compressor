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

#include "GRHistoryGraph.h"
#include "../LookAndFeel/Fonts.h"

GRHistoryGraph::GRHistoryGraph()
{
    rebuildCapacity();
}

void GRHistoryGraph::rebuildCapacity()
{
    int capacity = (int) (windowSeconds * (float) timerHz);
    historyL.setCapacity (capacity);
}

void GRHistoryGraph::setWindowSeconds (float seconds)
{
    windowSeconds = juce::jlimit (1.0f, 30.0f, seconds);
    rebuildCapacity();
}

void GRHistoryGraph::setTimerHz (int hz)
{
    timerHz = hz;
    rebuildCapacity();
}

void GRHistoryGraph::pushGainReduction (float grDB)
{
    historyL.push (grDB);
    repaint();
}

float GRHistoryGraph::grToY (float grDB, float height) const
{
    // 0 dB GR at top, -grRangeDB at bottom
    float normalized = juce::jlimit (0.0f, 1.0f, std::abs (grDB) / grRangeDB);
    return normalized * height;
}

void GRHistoryGraph::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);
    float w = bounds.getWidth();
    float h = bounds.getHeight();

    // Background
    g.setColour (ColourPalette::curveArea().withAlpha (0.5f));
    g.fillRoundedRectangle (bounds, 2.0f);

    // Grid lines (every 10 dB)
    g.setColour (ColourPalette::gridLine());
    for (float db = 10.0f; db < grRangeDB; db += 10.0f)
    {
        float y = bounds.getY() + grToY (-db, h);
        g.drawHorizontalLine ((int) y, bounds.getX(), bounds.getRight());
    }

    // 0 dB line at top
    g.setColour (ColourPalette::gridLineStrong());
    g.drawHorizontalLine ((int) bounds.getY(), bounds.getX(), bounds.getRight());

    int capacity = historyL.getCapacity();
    if (capacity < 2) return;

    // Build GR path
    juce::Path grPath;
    juce::Path fillPath;

    float xStep = w / (float) (capacity - 1);

    bool started = false;
    for (int i = 0; i < capacity; ++i)
    {
        float gr = historyL.getSample (i);
        float x = bounds.getX() + (float) i * xStep;
        float y = bounds.getY() + grToY (gr, h);

        if (!started)
        {
            grPath.startNewSubPath (x, y);
            fillPath.startNewSubPath (x, bounds.getY());
            fillPath.lineTo (x, y);
            started = true;
        }
        else
        {
            grPath.lineTo (x, y);
            fillPath.lineTo (x, y);
        }
    }

    // Close fill path back to top
    fillPath.lineTo (bounds.getRight(), bounds.getY());
    fillPath.closeSubPath();

    // Draw filled area
    g.setColour (ColourPalette::gainReductionFill());
    g.fillPath (fillPath);

    // Draw GR line
    g.setColour (ColourPalette::gainReduction());
    g.strokePath (grPath, juce::PathStrokeType (1.5f));

    // Labels
    g.setColour (ColourPalette::axisText());
    g.setFont (Fonts::getBody (9.0f));
    g.drawText ("0", bounds.withHeight (10.0f), juce::Justification::topLeft);

    auto labelBounds = bounds;
    g.drawText (juce::String (-(int) grRangeDB),
                labelBounds.removeFromBottom (10.0f), juce::Justification::bottomLeft);

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (1.0f), 2.0f, 0.5f);
}

void GRHistoryGraph::resized() {}

void GRHistoryGraph::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu())
    {
        juce::PopupMenu menu;
        menu.addItem (1, "3 seconds",  true, windowSeconds == 3.0f);
        menu.addItem (2, "5 seconds",  true, windowSeconds == 5.0f);
        menu.addItem (3, "10 seconds", true, windowSeconds == 10.0f);
        menu.addItem (4, "20 seconds", true, windowSeconds == 20.0f);
        menu.addItem (5, "30 seconds", true, windowSeconds == 30.0f);

        menu.showMenuAsync (juce::PopupMenu::Options(),
            [this] (int result)
            {
                switch (result)
                {
                    case 1: setWindowSeconds (3.0f);  break;
                    case 2: setWindowSeconds (5.0f);  break;
                    case 3: setWindowSeconds (10.0f); break;
                    case 4: setWindowSeconds (20.0f); break;
                    case 5: setWindowSeconds (30.0f); break;
                    default: break;
                }
            });
    }
}
