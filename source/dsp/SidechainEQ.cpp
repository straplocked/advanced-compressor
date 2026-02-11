#include "SidechainEQ.h"
#include "FilterTypes.h"

void SidechainEQ::prepare (double sampleRate)
{
    lowCutFilter.prepare (sampleRate);
    bandFilter.prepare (sampleRate);
    highCutFilter.prepare (sampleRate);
}

void SidechainEQ::reset()
{
    lowCutFilter.reset();
    bandFilter.reset();
    highCutFilter.reset();
}

void SidechainEQ::setLowCut (float freq, bool enabled)
{
    lcEnabled = enabled;
    if (enabled)
        lowCutFilter.setParameters (FilterType::HighPass, freq, 0.71, 0.0);
}

void SidechainEQ::setHighCut (float freq, bool enabled)
{
    hcEnabled = enabled;
    if (enabled)
        highCutFilter.setParameters (FilterType::LowPass, freq, 0.71, 0.0);
}

void SidechainEQ::setBandEQ (float freq, float gainDB)
{
    bandGain = gainDB;
    bandFilter.setParameters (FilterType::Bell, freq, 1.0, gainDB);
}

float SidechainEQ::processSample (float input)
{
    float out = input;
    if (lcEnabled)
        out = lowCutFilter.processSample (out);
    if (std::abs (bandGain) > 0.01f)
        out = bandFilter.processSample (out);
    if (hcEnabled)
        out = highCutFilter.processSample (out);
    return out;
}

void SidechainEQ::process (float* buffer, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        buffer[i] = processSample (buffer[i]);
}
