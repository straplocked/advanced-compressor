#pragma once

#include <juce_dsp/juce_dsp.h>
#include <memory>

class Oversampler
{
public:
    Oversampler() = default;

    void prepare (double sampleRate, int maxBlockSize, int numChannels);
    void reset();

    void setFactor (int factorIndex);  // 0=1x, 1=2x, 2=4x

    juce::dsp::AudioBlock<float> upsample (juce::AudioBuffer<float>& buffer);
    void downsample (juce::AudioBuffer<float>& buffer);

    int getLatencySamples() const;
    double getUpsampledRate() const { return upsampledRate; }
    int getFactor() const { return factor; }

private:
    void createOversampler (int maxBlockSize, int numChannels);

    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    int factor = 1;
    int factorIndex = 0;
    double baseSampleRate = 44100.0;
    double upsampledRate = 44100.0;
    int maxBlock = 512;
    int channels = 2;
};
