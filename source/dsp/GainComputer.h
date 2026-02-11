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

    // Returns gain reduction in dB (negative value) for a given input level in dB
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
};
