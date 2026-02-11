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

namespace Constants
{
    // Frequency range (for analyzer/sidechain display)
    constexpr float kMinFrequency = 10.0f;
    constexpr float kMaxFrequency = 30000.0f;

    // Editor
    constexpr int kDefaultEditorWidth = 1000;
    constexpr int kDefaultEditorHeight = 600;

    // Analyzer / FFT
    constexpr int kFFTOrder = 12;
    constexpr int kFFTSize = 1 << kFFTOrder;
    constexpr int kMaxFFTSize = 8192;
    constexpr float kAnalyzerMinDB = -100.0f;
    constexpr float kAnalyzerMaxDB = 6.0f;
    constexpr float kDefaultAnalyzerSmoothing = 0.7f;

    // UI
    constexpr int kUITimerHz = 30;
    constexpr int kCurveResolution = 512;

    // Compressor parameter ranges
    constexpr float kMinThreshold = -60.0f;
    constexpr float kMaxThreshold = 0.0f;
    constexpr float kDefaultThreshold = -20.0f;

    constexpr float kMinRatio = 1.0f;
    constexpr float kMaxRatio = 100.0f;
    constexpr float kDefaultRatio = 4.0f;

    constexpr float kMinKnee = 0.0f;
    constexpr float kMaxKnee = 72.0f;
    constexpr float kDefaultKnee = 6.0f;

    constexpr float kMinRange = 0.0f;
    constexpr float kMaxRange = 60.0f;
    constexpr float kDefaultRange = 60.0f;

    constexpr float kMinAttack = 0.005f;
    constexpr float kMaxAttack = 250.0f;
    constexpr float kDefaultAttack = 10.0f;

    constexpr float kMinRelease = 5.0f;
    constexpr float kMaxRelease = 2500.0f;
    constexpr float kDefaultRelease = 100.0f;

    constexpr float kMinHold = 0.0f;
    constexpr float kMaxHold = 500.0f;
    constexpr float kDefaultHold = 0.0f;

    constexpr float kMinLookahead = 0.0f;
    constexpr float kMaxLookahead = 20.0f;
    constexpr float kDefaultLookahead = 0.0f;

    constexpr float kMinGainDB = -30.0f;
    constexpr float kMaxGainDB = 30.0f;

    constexpr float kMinSCFreq = 20.0f;
    constexpr float kMaxSCFreq = 20000.0f;

    // Meter
    constexpr float kMeterMinDB = -60.0f;
    constexpr float kMeterMaxDB = 6.0f;
    constexpr float kGRMeterMinDB = -40.0f;
    constexpr float kGRMeterMaxDB = 0.0f;
    constexpr float kPeakHoldTimeMs = 1500.0f;
    constexpr float kPeakDecayDBPerSec = 20.0f;

    // Meter scale options (dB ranges)
    constexpr float kMeterScaleRanges[] = { 9.0f, 18.0f, 36.0f, 60.0f, 90.0f };
    constexpr int kDefaultMeterScaleIndex = 3;  // 60 dB

    // Max lookahead buffer (ms)
    constexpr float kMaxLookaheadMs = 20.0f;

    // Number of compressor styles
    constexpr int kNumStyles = 14;
}
