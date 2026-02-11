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

enum class CharacterMode
{
    Off = 0,
    Tube,
    Diode,
    Bright
};

class CharacterProcessor
{
public:
    CharacterProcessor() = default;

    void prepare (double sampleRate)
    {
        currentSampleRate = sampleRate;
    }

    void reset()
    {
        dcBlockState = 0.0f;
    }

    void setMode (CharacterMode mode) { currentMode = mode; }
    void setDrive (float driveDB) { driveGain = std::pow (10.0f, driveDB / 20.0f); }

    float processSample (float input)
    {
        if (currentMode == CharacterMode::Off)
            return input;

        float driven = input * driveGain;

        float shaped = driven;
        switch (currentMode)
        {
            case CharacterMode::Off:
                break;
            case CharacterMode::Tube:
                shaped = tubeWaveshaper (driven);
                break;
            case CharacterMode::Diode:
                shaped = diodeWaveshaper (driven);
                break;
            case CharacterMode::Bright:
                shaped = brightWaveshaper (driven);
                break;
        }

        // Compensate drive level
        shaped /= driveGain;

        // DC blocker
        float dcBlocked = shaped - dcBlockState + 0.9999f * dcBlockPrev;
        dcBlockState = shaped;
        dcBlockPrev = dcBlocked;

        return dcBlocked;
    }

private:
    // Soft tube saturation (tanh-like, asymmetric)
    static float tubeWaveshaper (float x)
    {
        // Warm asymmetric saturation -- positive half clips softer
        if (x >= 0.0f)
            return std::tanh (x * 0.8f) * 1.2f;
        else
            return std::tanh (x * 1.1f) * 0.9f;
    }

    // Diode clipping (harder, more aggressive)
    static float diodeWaveshaper (float x)
    {
        // Symmetric hard clipping with soft knee
        float absX = std::abs (x);
        if (absX < 0.5f)
            return x;
        float sign = x >= 0.0f ? 1.0f : -1.0f;
        return sign * (0.5f + (1.0f - std::exp (-(absX - 0.5f) * 3.0f)) / 3.0f);
    }

    // Bright character (adds high harmonics via polynomial)
    static float brightWaveshaper (float x)
    {
        // Chebyshev-like polynomial adding odd harmonics
        float x2 = x * x;
        return x * (1.0f + 0.15f * x2 - 0.05f * x2 * x2);
    }

    CharacterMode currentMode = CharacterMode::Off;
    float driveGain = 1.0f;
    double currentSampleRate = 44100.0;

    // DC blocker state
    float dcBlockState = 0.0f;
    float dcBlockPrev = 0.0f;
};
