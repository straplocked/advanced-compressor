#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "../analyzer/TripleBuffer.h"
#include "../utils/Constants.h"
#include <array>
#include <atomic>

struct MeterFrame
{
    float inputL = Constants::kMeterMinDB;
    float inputR = Constants::kMeterMinDB;
    float outputL = Constants::kMeterMinDB;
    float outputR = Constants::kMeterMinDB;
    float grL = 0.0f;
    float grR = 0.0f;
};

class LevelMeterData
{
public:
    LevelMeterData() = default;

    void prepare (double sampleRate);
    void reset();

    void pushInputLevel (const juce::AudioBuffer<float>& buffer);
    void pushOutputLevel (const juce::AudioBuffer<float>& buffer);
    void pushGainReduction (float grL, float grR);

    TripleBuffer<MeterFrame>& getBuffer() { return meterBuffer; }

private:
    void publishFrame();
    float computePeakDB (const float* data, int numSamples) const;

    TripleBuffer<MeterFrame> meterBuffer;
    MeterFrame currentFrame;
    double currentSampleRate = 44100.0;
};
