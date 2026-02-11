#pragma once

#include "GainComputer.h"
#include "LevelDetector.h"
#include "EnvelopeFollower.h"
#include "CompressorStyle.h"

// Per-channel compressor: detector -> gain computer -> envelope
class CompressorCore
{
public:
    CompressorCore() = default;

    void prepare (double sampleRate);
    void reset();

    void setStyle (CompressorStyle style);
    void setThreshold (float dB);
    void setRatio (float ratio);
    void setKnee (float dB);
    void setRange (float dB);
    void setAttack (float ms);
    void setRelease (float ms);
    void setHold (float ms);

    // Process sidechain sample, returns gain reduction in dB (negative)
    float processSidechain (float scSample);

    // Get current gain reduction for metering
    float getCurrentGR() const { return currentGR; }

private:
    LevelDetector detector;
    GainComputer gainComputer;
    EnvelopeFollower envelope;

    float currentGR = 0.0f;
    float feedbackGR = 0.0f;
    bool feedbackMode = false;
};
