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

    // Returns gain reduction in dB (negative value) for a given input level in dB
    float computeGainReduction (float inputDB) const
    {
        return CompressorMath::computeGainReduction (inputDB, threshold, ratio, knee, range);
    }

    float getThreshold() const { return threshold; }
    float getRatio() const { return ratio; }
    float getKnee() const { return knee; }

private:
    float threshold = -20.0f;
    float ratio = 4.0f;
    float knee = 6.0f;
    float range = 60.0f;
};
