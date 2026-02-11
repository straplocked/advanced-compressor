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

#include "CompressorStyle.h"
#include "../utils/CompressorMath.h"
#include <cmath>
#include <algorithm>

class EnvelopeFollower
{
public:
    EnvelopeFollower() = default;

    void prepare (double sampleRate);
    void reset();

    void setStyle (CompressorStyle style);
    void setAttack (float attackMs);
    void setRelease (float releaseMs);
    void setHold (float holdMs);

    // Process gain reduction value and apply envelope shaping
    // Input: target gain reduction in dB (negative)
    // Output: smoothed gain reduction in dB (negative)
    float processSample (float targetGR);

private:
    void updateCoefficients();

    double currentSampleRate = 44100.0;
    CompressorStyle currentStyle = CompressorStyle::Clean;
    StyleBehavior behavior {};

    float userAttackMs = 10.0f;
    float userReleaseMs = 100.0f;
    float userHoldMs = 0.0f;

    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
    int holdSamples = 0;

    float currentGR = 0.0f;  // current envelope state (dB, negative)
    int holdCounter = 0;

    // Opto two-stage release state
    float optoRelease1 = 0.0f;
    float optoRelease2 = 0.0f;

    // Program-dependent release state
    float prevTargetGR = 0.0f;

    // Op-El gain-dependent attack state
    float opElSlowAttack = 0.0f;
    float opElFastAttack = 0.0f;
};
