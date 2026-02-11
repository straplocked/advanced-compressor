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
    Pumping       // Peak, feedforward, exaggerated envelope
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
};

inline StyleBehavior getStyleBehavior (CompressorStyle style)
{
    switch (style)
    {
        case CompressorStyle::Clean:
            return { true, false, false, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, true };
        case CompressorStyle::Classic:
            return { false, true, true, 1.2f, 1.5f, 1.0f, 0.0f, 1.0f, true };
        case CompressorStyle::Opto:
            return { false, true, false, 2.0f, 3.0f, 0.0f, 3.0f, 0.8f, false };
        case CompressorStyle::Vocal:
            return { true, true, false, 0.8f, 1.0f, 1.0f, 4.0f, 0.9f, true };
        case CompressorStyle::Mastering:
            return { false, true, false, 1.5f, 1.2f, 1.0f, 6.0f, 1.0f, false };
        case CompressorStyle::Bus:
            return { false, true, true, 1.0f, 1.0f, 1.0f, 3.0f, 1.0f, true };
        case CompressorStyle::Punch:
            return { true, false, false, 0.3f, 0.5f, 2.0f, 0.0f, 1.0f, false };
        case CompressorStyle::Pumping:
            return { true, false, false, 0.5f, 2.5f, 0.0f, 0.0f, 1.2f, false };
    }
    return { true, false, false, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, false };
}
