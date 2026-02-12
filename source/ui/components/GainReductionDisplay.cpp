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

#include "GainReductionDisplay.h"
#include "../LookAndFeel/Fonts.h"

GainReductionDisplay::GainReductionDisplay() = default;

void GainReductionDisplay::setGainReduction (float gr) { gainReduction = gr; repaint(); }
void GainReductionDisplay::setLevels (float inp, float out) { inputLevel = inp; outputLevel = out; repaint(); }

void GainReductionDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);
    float w = bounds.getWidth();

    // "GR" heading at top
    float labelH = 12.0f;
    g.setColour (ColourPalette::axisText());
    g.setFont (Fonts::getBody (10.0f));
    g.drawText ("GR", bounds.withHeight (labelH), juce::Justification::centred, false);

    // Meter area below label
    auto meterBounds = bounds.withTrimmedTop (labelH + 1.0f);
    float meterH = meterBounds.getHeight();

    // Background
    g.setColour (ColourPalette::curveArea());
    g.fillRoundedRectangle (meterBounds, 2.0f);

    // GR bar (from top, growing downward — like Pro-C 3)
    float grNormalized = juce::jlimit (0.0f, 1.0f, std::abs (gainReduction) / 40.0f);
    float grHeight = grNormalized * meterH;
    if (grHeight > 0.5f)
    {
        g.setColour (ColourPalette::gainReductionFill());
        g.fillRect (meterBounds.getX(), meterBounds.getY(), w, grHeight);

        // GR line at the bottom edge of the bar
        g.setColour (ColourPalette::gainReduction());
        g.fillRect (meterBounds.getX(), meterBounds.getY() + grHeight - 1.5f, w, 1.5f);
    }

    // GR value text at bottom
    float textH = 14.0f;
    auto textBounds = meterBounds.removeFromBottom (textH);
    g.setColour (ColourPalette::textBright().withAlpha (0.8f));
    g.setFont (Fonts::getBody (9.0f));
    juce::String grText = juce::String (gainReduction, 1);
    g.drawText (grText, textBounds, juce::Justification::centred, false);

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRoundedRectangle (bounds.withTrimmedTop (labelH + 1.0f), 2.0f, 0.5f);
}
