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
    float lookaheadMs = 0.0f;
    int   style       = 0;
    float inputGain   = 0.0f;
    float outputGain  = 0.0f;
    float wetGain     = 0.0f;
    float mix         = 100.0f;
    bool  autoGain    = false;
    int   charMode    = 0;
    float charDrive   = 0.0f;
    bool  autoThresh  = false;
    float stereoLink  = 100.0f;
    int   stereoMode  = 0;
};

struct Preset
{
    juce::String           name;
    juce::String           category;
    CompressorPresetData   data;
    bool                   isFactory = true;
    bool                   isFavorite = false;
    juce::File             sourceFile;
};
