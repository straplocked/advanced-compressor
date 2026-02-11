#pragma once

#include <cmath>
#include <algorithm>
#include "../utils/MathUtils.h"

class LevelDetector
{
public:
    LevelDetector() = default;

    void prepare (double sampleRate);
    void reset();

    void setMode (bool usePeak, bool useRMS);

    // Process a single sample, returns level in dB
    float processSample (float input);

    // Set RMS window size in ms
    void setRMSWindowMs (float ms);

private:
    double currentSampleRate = 44100.0;
    bool peak = true;
    bool rms = false;

    // Peak detection state
    float peakLevel = 0.0f;

    // RMS state (simple one-pole)
    float rmsSquared = 0.0f;
    float rmsCoeff = 0.0f;
    float rmsWindowMs = 10.0f;
};
