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
#include "../../utils/Constants.h"

class LevelMeterComponent : public juce::Component
{
public:
    LevelMeterComponent();

    void paint (juce::Graphics& g) override;

    void setLevels (float leftDB, float rightDB);
    void setPeakHold (float leftPeak, float rightPeak);
    void setLabel (const juce::String& label) { meterLabel = label; }

private:
    float levelToY (float dB, float meterHeight) const;

    float leftLevel = Constants::kMeterMinDB;
    float rightLevel = Constants::kMeterMinDB;
    float leftPeak = Constants::kMeterMinDB;
    float rightPeak = Constants::kMeterMinDB;

    juce::String meterLabel;
};
