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

#include "../utils/CompressorMath.h"

class GainComputer
{
public:
    GainComputer() = default;

    void setParameters (float newThreshold, float newRatio, float newKnee, float newRange)
    {
        threshold = newThreshold;
        ratio = newRatio;
        knee = newKnee;
        range = newRange;
    }

    void setVariableRatio (bool enabled) { variableRatio = enabled; }
    void setUpwardMode (bool enabled) { upwardMode = enabled; }

    // Returns gain change in dB:
    //   Downward mode: negative value (gain reduction)
    //   Upward mode: positive value (gain boost)
    float computeGainReduction (float inputDB) const
    {
        float effectiveRatio = ratio;

        if (variableRatio)
        {
            // Vari-Mu: ratio increases with signal level above threshold
            float excess = inputDB - threshold;
            if (excess > 0.0f)
            {
                // Ratio ramps from 1.5:1 at threshold to user ratio at +20dB above
                float ramp = std::min (excess / 20.0f, 1.0f);
                effectiveRatio = 1.5f + ramp * (ratio - 1.5f);
            }
            else
            {
                effectiveRatio = 1.5f;
            }
        }

        if (upwardMode)
            return CompressorMath::computeUpwardGain (inputDB, threshold, effectiveRatio, knee, range);

        return CompressorMath::computeGainReduction (inputDB, threshold, effectiveRatio, knee, range);
    }

    float getThreshold() const { return threshold; }
    float getRatio() const { return ratio; }
    float getKnee() const { return knee; }

private:
    float threshold = -20.0f;
    float ratio = 4.0f;
    float knee = 6.0f;
    float range = 60.0f;
    bool variableRatio = false;
    bool upwardMode = false;
};
