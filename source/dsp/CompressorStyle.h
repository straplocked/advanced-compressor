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

enum class CompressorStyle
{
    Clean = 0,    // Peak, feedforward, low distortion, program-dependent release
    Classic,      // RMS, feedback, vintage character
    Opto,         // RMS, feedforward, non-linear two-stage release (LDR model)
    Vocal,        // Peak+RMS, auto knee/ratio tweaks
    Mastering,    // RMS, feedforward, maximum transparency
    Bus,          // RMS, feedback, glue character
    Punch,        // Peak, feedforward, fast, preserves transients
    Pumping,      // Peak, feedforward, exaggerated envelope
    Versatile,    // Balanced Peak+RMS, general purpose
    Smooth,       // Super-smooth gluing, slow RMS, large knee
    OpEl,         // Warm opto-like tube compression, gain-dependent attack
    VariMu,       // Variable-mu: ratio varies with signal level
    Upward,       // Upward compression: boosts signals below threshold
    TTM           // To The Max: combined downward + upward compression
};

struct StyleBehavior
{
    bool usePeak;        // true=peak, false=RMS
    bool useRMS;         // true=include RMS
    bool feedback;       // true=feedback topology
    float attackScale;   // multiplier on user attack
    float releaseScale;  // multiplier on user release
    float holdScale;     // multiplier on user hold
    float kneeOffset;    // added to user knee (dB)
    float ratioScale;    // multiplier on user ratio
    bool programDepRelease; // enable program-dependent release
    bool gainDepAttack;     // enable gain-dependent attack (Op-El)
    bool variableRatio;     // enable variable ratio (Vari-Mu)
    bool upward;            // enable upward compression (boost below threshold)
    bool ttm;               // enable TTM (downward + upward combined)
};

inline StyleBehavior getStyleBehavior (CompressorStyle style)
{
    switch (style)
    {
        case CompressorStyle::Clean:
            return { true, false, false, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, true, false, false, false, false };
        case CompressorStyle::Classic:
            return { false, true, true, 1.2f, 1.5f, 1.0f, 0.0f, 1.0f, true, false, false, false, false };
        case CompressorStyle::Opto:
            return { false, true, false, 2.0f, 3.0f, 0.0f, 3.0f, 0.8f, false, false, false, false, false };
        case CompressorStyle::Vocal:
            return { true, true, false, 0.8f, 1.0f, 1.0f, 4.0f, 0.9f, true, false, false, false, false };
        case CompressorStyle::Mastering:
            return { false, true, false, 1.5f, 1.2f, 1.0f, 6.0f, 1.0f, false, false, false, false, false };
        case CompressorStyle::Bus:
            return { false, true, true, 1.0f, 1.0f, 1.0f, 3.0f, 1.0f, true, false, false, false, false };
        case CompressorStyle::Punch:
            return { true, false, false, 0.3f, 0.5f, 2.0f, 0.0f, 1.0f, false, false, false, false, false };
        case CompressorStyle::Pumping:
            return { true, false, false, 0.5f, 2.5f, 0.0f, 0.0f, 1.2f, false, false, false, false, false };
        case CompressorStyle::Versatile:
            return { true, true, false, 1.0f, 1.0f, 1.0f, 2.0f, 1.0f, true, false, false, false, false };
        case CompressorStyle::Smooth:
            return { false, true, false, 2.5f, 2.0f, 0.0f, 12.0f, 0.7f, false, false, false, false, false };
        case CompressorStyle::OpEl:
            return { false, true, false, 1.5f, 2.5f, 0.0f, 4.0f, 0.85f, false, true, false, false, false };
        case CompressorStyle::VariMu:
            return { false, true, true, 1.8f, 1.5f, 1.0f, 8.0f, 1.0f, true, false, true, false, false };
        case CompressorStyle::Upward:
            return { false, true, false, 1.0f, 1.2f, 1.0f, 4.0f, 1.0f, true, false, false, true, false };
        case CompressorStyle::TTM:
            return { false, true, false, 1.0f, 1.0f, 1.0f, 4.0f, 1.0f, true, false, false, false, true };
    }
    return { true, false, false, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, false, false, false, false, false };
}
