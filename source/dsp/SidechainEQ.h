#pragma once

#include "SVFFilter.h"

class SidechainEQ
{
public:
    SidechainEQ() = default;

    void prepare (double sampleRate);
    void reset();

    void setLowCut (float freq, bool enabled);
    void setHighCut (float freq, bool enabled);
    void setBandEQ (float freq, float gainDB);

    float processSample (float input);
    void process (float* buffer, int numSamples);

private:
    SVFFilter lowCutFilter;
    SVFFilter bandFilter;
    SVFFilter highCutFilter;

    bool lcEnabled = false;
    bool hcEnabled = false;
    float bandGain = 0.0f;
};
