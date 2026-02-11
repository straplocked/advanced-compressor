#pragma once

#include <juce_core/juce_core.h>

namespace ParamIDs
{
    // Dynamics
    constexpr const char* kThreshold  = "threshold";
    constexpr const char* kRatio      = "ratio";
    constexpr const char* kKnee       = "knee";
    constexpr const char* kRange      = "range";
    constexpr const char* kAttack     = "attack";
    constexpr const char* kRelease    = "release";
    constexpr const char* kHold       = "hold";
    constexpr const char* kLookahead  = "lookahead";

    // Style
    constexpr const char* kCompStyle  = "comp_style";

    // Gain/Mix
    constexpr const char* kInputGain  = "input_gain";
    constexpr const char* kInputPan   = "input_pan";
    constexpr const char* kOutputGain = "output_gain";
    constexpr const char* kOutputPan  = "output_pan";
    constexpr const char* kWetGain    = "wet_gain";
    constexpr const char* kAutoGain   = "auto_gain";
    constexpr const char* kMix        = "mix";

    // Sidechain
    constexpr const char* kSCExternal  = "sc_external";
    constexpr const char* kSCAudition  = "sc_audition";
    constexpr const char* kSCLCFreq    = "sc_lc_freq";
    constexpr const char* kSCLCOn      = "sc_lc_on";
    constexpr const char* kSCBandFreq  = "sc_band_freq";
    constexpr const char* kSCBandGain  = "sc_band_gain";
    constexpr const char* kSCHCFreq    = "sc_hc_freq";
    constexpr const char* kSCHCOn      = "sc_hc_on";

    // Stereo
    constexpr const char* kStereoLink  = "stereo_link";
    constexpr const char* kStereoMode  = "stereo_mode";

    // Display/Global
    constexpr const char* kAnalyzerOn  = "analyzer_on";
    constexpr const char* kAnalyzerPre = "analyzer_pre";
    constexpr const char* kOversample  = "oversample";
    constexpr const char* kCompactMode = "compact_mode";

    // Name lists
    inline juce::StringArray getStyleNames()
    {
        return { "Clean", "Classic", "Opto", "Vocal", "Mastering", "Bus", "Punch", "Pumping" };
    }

    inline juce::StringArray getStereoModeNames()
    {
        return { "Normal", "Mid", "Side" };
    }

    inline juce::StringArray getOversamplingNames()
    {
        return { "1x", "2x", "4x" };
    }
}
