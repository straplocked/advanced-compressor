#include "ParameterLayout.h"
#include "ParameterIDs.h"
#include "../utils/Constants.h"

namespace ParameterLayout
{

juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // ---- Dynamics ----

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kThreshold, 1 },
        "Threshold",
        juce::NormalisableRange<float> (Constants::kMinThreshold, Constants::kMaxThreshold, 0.1f, 1.0f),
        Constants::kDefaultThreshold));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kRatio, 1 },
        "Ratio",
        juce::NormalisableRange<float> (Constants::kMinRatio, Constants::kMaxRatio, 0.01f, 0.3f),
        Constants::kDefaultRatio));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kKnee, 1 },
        "Knee",
        juce::NormalisableRange<float> (Constants::kMinKnee, Constants::kMaxKnee, 0.1f, 0.5f),
        Constants::kDefaultKnee));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kRange, 1 },
        "Range",
        juce::NormalisableRange<float> (Constants::kMinRange, Constants::kMaxRange, 0.1f, 0.5f),
        Constants::kDefaultRange));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kAttack, 1 },
        "Attack",
        juce::NormalisableRange<float> (Constants::kMinAttack, Constants::kMaxAttack, 0.001f, 0.3f),
        Constants::kDefaultAttack));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kRelease, 1 },
        "Release",
        juce::NormalisableRange<float> (Constants::kMinRelease, Constants::kMaxRelease, 0.1f, 0.3f),
        Constants::kDefaultRelease));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kHold, 1 },
        "Hold",
        juce::NormalisableRange<float> (Constants::kMinHold, Constants::kMaxHold, 0.1f, 0.4f),
        Constants::kDefaultHold));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kLookahead, 1 },
        "Lookahead",
        juce::NormalisableRange<float> (Constants::kMinLookahead, Constants::kMaxLookahead, 0.01f, 0.5f),
        Constants::kDefaultLookahead));

    // ---- Character ----

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kCharMode, 1 },
        "Character",
        ParamIDs::getCharacterModeNames(),
        0));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kCharDrive, 1 },
        "Drive",
        juce::NormalisableRange<float> (0.0f, 24.0f, 0.1f),
        0.0f));

    // ---- Auto Threshold ----

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kAutoThresh, 1 },
        "Auto Threshold",
        false));

    // ---- Style ----

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kCompStyle, 1 },
        "Style",
        ParamIDs::getStyleNames(),
        0));

    // ---- Gain/Mix ----

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kInputGain, 1 },
        "Input Gain",
        juce::NormalisableRange<float> (Constants::kMinGainDB, Constants::kMaxGainDB, 0.01f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kInputPan, 1 },
        "Input Pan",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kOutputGain, 1 },
        "Output Gain",
        juce::NormalisableRange<float> (Constants::kMinGainDB, Constants::kMaxGainDB, 0.01f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kOutputPan, 1 },
        "Output Pan",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kWetGain, 1 },
        "Wet Gain",
        juce::NormalisableRange<float> (Constants::kMinGainDB, Constants::kMaxGainDB, 0.01f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kAutoGain, 1 },
        "Auto Gain",
        false));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kMix, 1 },
        "Mix",
        juce::NormalisableRange<float> (0.0f, 200.0f, 0.1f),
        100.0f));

    // ---- Sidechain ----

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kSCExternal, 1 },
        "SC External",
        false));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kSCAudition, 1 },
        "SC Audition",
        false));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kDelta, 1 },
        "Delta",
        false));

    // SC Low Cut
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCLCFreq, 1 },
        "SC Low Cut",
        juce::NormalisableRange<float> (Constants::kMinSCFreq, Constants::kMaxSCFreq, 0.0f, 0.25f),
        20.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kSCLCOn, 1 },
        "SC LC On",
        false));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kSCLCSlope, 1 },
        "SC LC Slope",
        ParamIDs::getSCSlopeNames(),
        1));  // default 12 dB/oct

    // SC Low Shelf
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCLSFreq, 1 },
        "SC Low Shelf Freq",
        juce::NormalisableRange<float> (Constants::kMinSCFreq, Constants::kMaxSCFreq, 0.0f, 0.25f),
        200.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCLSGain, 1 },
        "SC Low Shelf Gain",
        juce::NormalisableRange<float> (-15.0f, 15.0f, 0.1f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kSCLSOn, 1 },
        "SC LS On",
        false));

    // SC Band 1
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCBandFreq, 1 },
        "SC Band Freq",
        juce::NormalisableRange<float> (Constants::kMinSCFreq, Constants::kMaxSCFreq, 0.0f, 0.25f),
        1000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCBandGain, 1 },
        "SC Band Gain",
        juce::NormalisableRange<float> (-15.0f, 15.0f, 0.1f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCBandQ, 1 },
        "SC Band Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.01f, 0.4f),
        1.0f));

    // SC Band 2
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCBand2Freq, 1 },
        "SC Band 2 Freq",
        juce::NormalisableRange<float> (Constants::kMinSCFreq, Constants::kMaxSCFreq, 0.0f, 0.25f),
        3000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCBand2Gain, 1 },
        "SC Band 2 Gain",
        juce::NormalisableRange<float> (-15.0f, 15.0f, 0.1f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCBand2Q, 1 },
        "SC Band 2 Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.01f, 0.4f),
        1.0f));

    // SC High Shelf
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCHSFreq, 1 },
        "SC High Shelf Freq",
        juce::NormalisableRange<float> (Constants::kMinSCFreq, Constants::kMaxSCFreq, 0.0f, 0.25f),
        8000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCHSGain, 1 },
        "SC High Shelf Gain",
        juce::NormalisableRange<float> (-15.0f, 15.0f, 0.1f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kSCHSOn, 1 },
        "SC HS On",
        false));

    // SC High Cut
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kSCHCFreq, 1 },
        "SC High Cut",
        juce::NormalisableRange<float> (Constants::kMinSCFreq, Constants::kMaxSCFreq, 0.0f, 0.25f),
        20000.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kSCHCOn, 1 },
        "SC HC On",
        false));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kSCHCSlope, 1 },
        "SC HC Slope",
        ParamIDs::getSCSlopeNames(),
        1));  // default 12 dB/oct

    // ---- Stereo ----

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::kStereoLink, 1 },
        "Stereo Link",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        100.0f));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kStereoMode, 1 },
        "Stereo Mode",
        ParamIDs::getStereoModeNames(),
        0));

    // ---- MIDI Trigger / Host Sync ----

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kMIDITrigger, 1 },
        "MIDI Trigger",
        false));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kHostSync, 1 },
        "Host Sync",
        false));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kSyncDiv, 1 },
        "Sync Division",
        ParamIDs::getSyncDivisionNames(),
        2));  // default 1/4

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kSyncShape, 1 },
        "Sync Shape",
        ParamIDs::getSyncShapeNames(),
        0));  // default Square

    // ---- Display/Global ----

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kAnalyzerOn, 1 },
        "Analyzer",
        true));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kAnalyzerPre, 1 },
        "Analyzer Pre",
        false));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kOversample, 1 },
        "Oversampling",
        ParamIDs::getOversamplingNames(),
        0));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::kCompactMode, 1 },
        "Compact Mode",
        false));

    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { ParamIDs::kMeterScale, 1 },
        "Meter Scale",
        ParamIDs::getMeterScaleNames(),
        Constants::kDefaultMeterScaleIndex));

    return layout;
}

} // namespace ParameterLayout
