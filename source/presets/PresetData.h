#pragma once

#include <juce_core/juce_core.h>
#include <vector>

struct CompressorPresetData
{
    float threshold   = -20.0f;
    float ratio       = 4.0f;
    float knee        = 6.0f;
    float range       = 60.0f;
    float attackMs    = 10.0f;
    float releaseMs   = 100.0f;
    float holdMs      = 0.0f;
    int   style       = 0;
    float mix         = 100.0f;
    float outputGain  = 0.0f;
};

struct Preset
{
    juce::String           name;
    juce::String           category;
    CompressorPresetData   data;
    bool                   isFactory = true;
    juce::File             sourceFile;
};
