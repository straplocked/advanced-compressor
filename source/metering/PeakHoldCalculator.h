#pragma once

#include "../utils/Constants.h"
#include <algorithm>

class PeakHoldCalculator
{
public:
    PeakHoldCalculator() = default;

    void prepare (double sampleRate)
    {
        holdSamples = (int)(Constants::kPeakHoldTimeMs * 0.001 * sampleRate);
        decayPerSample = Constants::kPeakDecayDBPerSec / (float) sampleRate;
        reset();
    }

    void reset()
    {
        peakValue = Constants::kMeterMinDB;
        holdCounter = 0;
    }

    float process (float inputDB)
    {
        if (inputDB >= peakValue)
        {
            peakValue = inputDB;
            holdCounter = holdSamples;
        }
        else if (holdCounter > 0)
        {
            --holdCounter;
        }
        else
        {
            peakValue -= decayPerSample;
            peakValue = std::max (peakValue, Constants::kMeterMinDB);
        }

        return peakValue;
    }

private:
    float peakValue = Constants::kMeterMinDB;
    int holdCounter = 0;
    int holdSamples = 0;
    float decayPerSample = 0.0f;
};
