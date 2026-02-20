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
#include "GRHistoryBuffer.h"

class GRHistoryGraph : public juce::Component
{
public:
    GRHistoryGraph();

    void paint (juce::Graphics& g) override;
    void resized() override;

    void pushGainReduction (float grDB);
    void setWindowSeconds (float seconds);
    void setTimerHz (int hz);

    void mouseDown (const juce::MouseEvent& e) override;

private:
    float grToY (float grDB, float height) const;
    void rebuildCapacity();

    GRHistoryBuffer historyL;
    float windowSeconds = 5.0f;
    int timerHz = Constants::kUITimerHz;
    float grRangeDB = 40.0f;
};
