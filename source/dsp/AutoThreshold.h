#pragma once

#include <cmath>
#include <algorithm>

// Slow RMS follower that tracks average input level.
// When enabled, the effective threshold = user threshold + offset from average level.
class AutoThreshold
{
public:
    AutoThreshold() = default;

    void prepare (double sampleRate)
    {
        // Very slow follower: ~2 second time constant
        float timeMs = 2000.0f;
        coeff = std::exp (-1.0f / (float) (timeMs * 0.001 * sampleRate));
        reset();
    }

    void reset()
    {
        avgLevelDB = -40.0f;
    }

    void setEnabled (bool e) { enabled = e; }

    // Push the current block's peak level (in dB)
    void pushLevel (float peakDB)
    {
        if (!enabled)
            return;

        // Slow one-pole average
        avgLevelDB = coeff * avgLevelDB + (1.0f - coeff) * peakDB;
    }

    // Returns the offset to apply to the user threshold
    float getThresholdOffset() const
    {
        if (!enabled)
            return 0.0f;

        // Target: threshold tracks so it sits ~20dB above the average level
        // Offset = average level - reference level (-20 dB is our reference)
        float referenceLevel = -20.0f;
        return avgLevelDB - referenceLevel;
    }

private:
    bool enabled = false;
    float coeff = 0.999f;
    float avgLevelDB = -40.0f;
};
