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

#include "LevelMeterComponent.h"

LevelMeterComponent::LevelMeterComponent() = default;

void LevelMeterComponent::setLevels (float l, float r)
{
    leftLevel = l;
    rightLevel = r;
    repaint();
}

void LevelMeterComponent::setPeakHold (float lp, float rp)
{
    leftPeak = lp;
    rightPeak = rp;
}

float LevelMeterComponent::levelToY (float dB, float meterHeight) const
{
    float normalized = (dB - Constants::kMeterMinDB) / (Constants::kMeterMaxDB - Constants::kMeterMinDB);
    return meterHeight * (1.0f - juce::jlimit (0.0f, 1.0f, normalized));
}

void LevelMeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float labelHeight = meterLabel.isNotEmpty() ? 12.0f : 0.0f;

    // Draw label at top
    if (meterLabel.isNotEmpty())
    {
        g.setColour (ColourPalette::axisText());
        g.setFont (9.0f);
        g.drawText (meterLabel, bounds.removeFromTop (labelHeight),
                    juce::Justification::centred, false);
    }

    float meterWidth = (bounds.getWidth() - 2.0f) * 0.5f;

    // Background
    g.setColour (ColourPalette::curveArea());
    g.fillRoundedRectangle (bounds, 2.0f);

    float h = bounds.getHeight();
    float meterTop = bounds.getY();

    auto drawMeter = [&] (float x, float dB, float peak)
    {
        float meterY = levelToY (dB, h);

        // Gradient meter bar
        juce::ColourGradient gradient (ColourPalette::meterGreen(), x, meterTop + h,
                                        ColourPalette::meterRed(), x, meterTop, false);
        gradient.addColour (0.6, ColourPalette::meterYellow());
        g.setGradientFill (gradient);
        g.fillRect (x, meterTop + meterY, meterWidth, h - meterY);

        // Peak hold indicator
        if (peak > Constants::kMeterMinDB + 1.0f)
        {
            float peakY = levelToY (peak, h);
            g.setColour (ColourPalette::textBright());
            g.fillRect (x, meterTop + peakY, meterWidth, 1.0f);
        }
    };

    drawMeter (bounds.getX(), leftLevel, leftPeak);
    drawMeter (bounds.getX() + meterWidth + 2.0f, rightLevel, rightPeak);

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRoundedRectangle (bounds, 2.0f, 0.5f);
}
