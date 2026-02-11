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

#include <juce_graphics/juce_graphics.h>
#include "FFTProcessor.h"
#include "../utils/Constants.h"
#include "../utils/MathUtils.h"
#include <vector>

class SpectrumAnalyzer
{
public:
    SpectrumAnalyzer();

    void prepare (int numDisplayPoints);
    bool update (FFTProcessor& fftProcessor, float displayWidth, float displayHeight);

    const juce::Path& getPath() const { return spectrumPath; }
    const juce::Path& getFillPath() const { return spectrumFillPath; }

    void setSmoothing (float s) { smoothingCoeff = juce::jlimit (0.0f, 0.99f, s); }

private:
    float spectrumDBtoY (float dB, float height) const;

    float smoothingCoeff = Constants::kDefaultAnalyzerSmoothing;
    int numPoints = 0;
    std::vector<float> smoothedMagnitudes;
    std::vector<float> displayFrequencies;
    juce::Path spectrumPath;
    juce::Path spectrumFillPath;
};
