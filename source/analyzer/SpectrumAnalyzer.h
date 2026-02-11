#pragma once

#include <juce_graphics/juce_graphics.h>
#include "FFTProcessor.h"
#include "../utils/Constants.h"
#include "../utils/MathUtils.h"
#include <vector>

class SpectrumAnalyzer
{
public:
    SpectrumAnalyzer();

    void prepare (int numDisplayPoints);
    bool update (FFTProcessor& fftProcessor, float displayWidth, float displayHeight);

    const juce::Path& getPath() const { return spectrumPath; }
    const juce::Path& getFillPath() const { return spectrumFillPath; }

    void setSmoothing (float s) { smoothingCoeff = juce::jlimit (0.0f, 0.99f, s); }

private:
    float spectrumDBtoY (float dB, float height) const;

    float smoothingCoeff = Constants::kDefaultAnalyzerSmoothing;
    int numPoints = 0;
    std::vector<float> smoothedMagnitudes;
    std::vector<float> displayFrequencies;
    juce::Path spectrumPath;
    juce::Path spectrumFillPath;
};
