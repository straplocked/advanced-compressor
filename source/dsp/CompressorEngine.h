#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "CompressorCore.h"
#include "SidechainEQ.h"
#include "Oversampler.h"
#include "AutoGainComputer.h"
#include "MidSideProcessor.h"
#include "../parameters/CompressorParameterAttachment.h"
#include "../metering/LevelMeterData.h"
#include <vector>
#include <array>

class CompressorEngine
{
public:
    CompressorEngine();

    void prepare (double sampleRate, int maxBlockSize, int numChannels);
    void reset();

    void updateParameters (const CompressorParameters& params);
    void process (juce::AudioBuffer<float>& buffer,
                  const juce::AudioBuffer<float>* sidechainBuffer);

    LevelMeterData& getMeterData() { return meterData; }
    float getCurrentGR() const;

    int getLatencySamples() const;

private:
    void applyInputGain (juce::AudioBuffer<float>& buffer);
    void applyOutputGain (juce::AudioBuffer<float>& buffer);
    void applyPan (juce::AudioBuffer<float>& buffer, float pan);

    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;

    // Per-channel compressor cores (L, R)
    std::array<CompressorCore, 2> cores;
    std::array<SidechainEQ, 2> sidechainEQ;

    Oversampler oversampler;
    AutoGainComputer autoGainComputer;

    // Lookahead delay line
    std::vector<std::vector<float>> lookaheadBuffer;
    int lookaheadWritePos = 0;
    int lookaheadSamples = 0;

    // Dry buffer for mix
    juce::AudioBuffer<float> dryBuffer;

    // Current parameters (cached)
    CompressorParameters cachedParams;

    // Metering
    LevelMeterData meterData;
};
