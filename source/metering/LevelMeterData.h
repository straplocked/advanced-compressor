/*
 * Advanced Compressor - Open-source dynamic range compressor plugin
 * Copyright (C) 2024-2026 Advanced Compressor contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "../analyzer/TripleBuffer.h"
#include "../utils/Constants.h"
#include "LoudnessMeter.h"
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
    float momentaryLUFS = -100.0f;
    float shortTermLUFS = -100.0f;
    float integratedLUFS = -100.0f;
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
    LoudnessMeter loudnessMeter;
    double currentSampleRate = 44100.0;
};
