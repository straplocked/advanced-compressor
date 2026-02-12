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

#include "SidechainMeterRing.h"

SidechainMeterRing::SidechainMeterRing()
{
    setInterceptsMouseClicks (false, false);
}

void SidechainMeterRing::setGainReduction (float gr)
{
    gainReduction = gr;
    repaint();
}

void SidechainMeterRing::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (2.0f);
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY();
    float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;

    // Track ring (background)
    float ringWidth = 3.0f;
    float startAngle = juce::MathConstants<float>::pi * 0.75f;
    float endAngle = juce::MathConstants<float>::pi * 2.25f;

    juce::Path trackPath;
    trackPath.addCentredArc (cx, cy, radius - ringWidth, radius - ringWidth,
                             0.0f, startAngle, endAngle, true);
    g.setColour (ColourPalette::gridLine());
    g.strokePath (trackPath, juce::PathStrokeType (ringWidth));

    // GR arc (from end going backward)
    float grNormalized = juce::jlimit (0.0f, 1.0f, std::abs (gainReduction) / 30.0f);
    if (grNormalized > 0.001f)
    {
        float grAngle = endAngle - grNormalized * (endAngle - startAngle);
        juce::Path grPath;
        grPath.addCentredArc (cx, cy, radius - ringWidth, radius - ringWidth,
                              0.0f, grAngle, endAngle, true);
        g.setColour (ColourPalette::gainReduction());
        g.strokePath (grPath, juce::PathStrokeType (ringWidth));
    }
}
