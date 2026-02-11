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

    // Process sidechain sample, returns gain change in dB
    // Downward: negative (reduction), Upward: positive (boost), TTM: combined
    float processSidechain (float scSample);

    // Get current gain reduction for metering
    float getCurrentGR() const { return currentGR; }

private:
    LevelDetector detector;
    GainComputer gainComputer;
    EnvelopeFollower envelope;

    // Second gain computer + envelope for TTM (upward part)
    GainComputer upwardGainComputer;
    EnvelopeFollower upwardEnvelope;

    float currentGR = 0.0f;
    float feedbackGR = 0.0f;
    bool feedbackMode = false;
    bool upwardMode = false;
    bool ttmMode = false;
};
