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

#include <cmath>
#include <algorithm>

class AutoGainComputer
{
public:
    AutoGainComputer() = default;

    void prepare (double sampleRate)
    {
        currentSampleRate = sampleRate;
        reset();
    }

    void reset()
    {
        smoothedGR = 0.0f;
    }

    // Feed current gain reduction (dB, negative) to track average
    void pushGainReduction (float grDB)
    {
        float coeff = std::exp (-1.0f / (float)(0.5 * currentSampleRate));
        smoothedGR = coeff * smoothedGR + (1.0f - coeff) * grDB;
    }

    // Returns makeup gain estimate in dB (positive)
    float getAutoGainDB() const
    {
        return -smoothedGR * 0.5f;  // Compensate ~50% of average GR
    }

private:
    double currentSampleRate = 44100.0;
    float smoothedGR = 0.0f;
};
