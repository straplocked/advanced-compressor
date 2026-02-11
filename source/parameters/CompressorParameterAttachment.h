#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "ParameterIDs.h"

struct CompressorParameters
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
    float inputPan    = 0.0f;
    float outputGain  = 0.0f;
    float outputPan   = 0.0f;
    float wetGain     = 0.0f;
    bool  autoGain    = false;
    float mix         = 100.0f;

    bool  scExternal  = false;
    bool  scAudition  = false;
    float scLCFreq    = 20.0f;
    bool  scLCOn      = false;
    float scBandFreq  = 1000.0f;
    float scBandGain  = 0.0f;
    float scHCFreq    = 20000.0f;
    bool  scHCOn      = false;

    float stereoLink  = 100.0f;
    int   stereoMode  = 0;

    bool  analyzerOn  = true;
    bool  analyzerPre = false;
    int   oversample  = 0;
    bool  compactMode = false;
};

class CompressorParameterAttachment
{
public:
    CompressorParameterAttachment() = default;

    void init (juce::AudioProcessorValueTreeState& apvts)
    {
        rawThreshold   = apvts.getRawParameterValue (ParamIDs::kThreshold);
        rawRatio       = apvts.getRawParameterValue (ParamIDs::kRatio);
        rawKnee        = apvts.getRawParameterValue (ParamIDs::kKnee);
        rawRange       = apvts.getRawParameterValue (ParamIDs::kRange);
        rawAttack      = apvts.getRawParameterValue (ParamIDs::kAttack);
        rawRelease     = apvts.getRawParameterValue (ParamIDs::kRelease);
        rawHold        = apvts.getRawParameterValue (ParamIDs::kHold);
        rawLookahead   = apvts.getRawParameterValue (ParamIDs::kLookahead);
        rawStyle       = apvts.getRawParameterValue (ParamIDs::kCompStyle);

        rawInputGain   = apvts.getRawParameterValue (ParamIDs::kInputGain);
        rawInputPan    = apvts.getRawParameterValue (ParamIDs::kInputPan);
        rawOutputGain  = apvts.getRawParameterValue (ParamIDs::kOutputGain);
        rawOutputPan   = apvts.getRawParameterValue (ParamIDs::kOutputPan);
        rawWetGain     = apvts.getRawParameterValue (ParamIDs::kWetGain);
        rawAutoGain    = apvts.getRawParameterValue (ParamIDs::kAutoGain);
        rawMix         = apvts.getRawParameterValue (ParamIDs::kMix);

        rawSCExternal  = apvts.getRawParameterValue (ParamIDs::kSCExternal);
        rawSCAudition  = apvts.getRawParameterValue (ParamIDs::kSCAudition);
        rawSCLCFreq    = apvts.getRawParameterValue (ParamIDs::kSCLCFreq);
        rawSCLCOn      = apvts.getRawParameterValue (ParamIDs::kSCLCOn);
        rawSCBandFreq  = apvts.getRawParameterValue (ParamIDs::kSCBandFreq);
        rawSCBandGain  = apvts.getRawParameterValue (ParamIDs::kSCBandGain);
        rawSCHCFreq    = apvts.getRawParameterValue (ParamIDs::kSCHCFreq);
        rawSCHCOn      = apvts.getRawParameterValue (ParamIDs::kSCHCOn);

        rawStereoLink  = apvts.getRawParameterValue (ParamIDs::kStereoLink);
        rawStereoMode  = apvts.getRawParameterValue (ParamIDs::kStereoMode);

        rawAnalyzerOn  = apvts.getRawParameterValue (ParamIDs::kAnalyzerOn);
        rawAnalyzerPre = apvts.getRawParameterValue (ParamIDs::kAnalyzerPre);
        rawOversample  = apvts.getRawParameterValue (ParamIDs::kOversample);
        rawCompactMode = apvts.getRawParameterValue (ParamIDs::kCompactMode);
    }

    CompressorParameters getCurrentParameters() const
    {
        CompressorParameters p;
        p.threshold   = rawThreshold->load();
        p.ratio       = rawRatio->load();
        p.knee        = rawKnee->load();
        p.range       = rawRange->load();
        p.attackMs    = rawAttack->load();
        p.releaseMs   = rawRelease->load();
        p.holdMs      = rawHold->load();
        p.lookaheadMs = rawLookahead->load();
        p.style       = (int) rawStyle->load();

        p.inputGain   = rawInputGain->load();
        p.inputPan    = rawInputPan->load();
        p.outputGain  = rawOutputGain->load();
        p.outputPan   = rawOutputPan->load();
        p.wetGain     = rawWetGain->load();
        p.autoGain    = rawAutoGain->load() >= 0.5f;
        p.mix         = rawMix->load();

        p.scExternal  = rawSCExternal->load() >= 0.5f;
        p.scAudition  = rawSCAudition->load() >= 0.5f;
        p.scLCFreq    = rawSCLCFreq->load();
        p.scLCOn      = rawSCLCOn->load() >= 0.5f;
        p.scBandFreq  = rawSCBandFreq->load();
        p.scBandGain  = rawSCBandGain->load();
        p.scHCFreq    = rawSCHCFreq->load();
        p.scHCOn      = rawSCHCOn->load() >= 0.5f;

        p.stereoLink  = rawStereoLink->load();
        p.stereoMode  = (int) rawStereoMode->load();

        p.analyzerOn  = rawAnalyzerOn->load() >= 0.5f;
        p.analyzerPre = rawAnalyzerPre->load() >= 0.5f;
        p.oversample  = (int) rawOversample->load();
        p.compactMode = rawCompactMode->load() >= 0.5f;
        return p;
    }

private:
    std::atomic<float>* rawThreshold   = nullptr;
    std::atomic<float>* rawRatio       = nullptr;
    std::atomic<float>* rawKnee        = nullptr;
    std::atomic<float>* rawRange       = nullptr;
    std::atomic<float>* rawAttack      = nullptr;
    std::atomic<float>* rawRelease     = nullptr;
    std::atomic<float>* rawHold        = nullptr;
    std::atomic<float>* rawLookahead   = nullptr;
    std::atomic<float>* rawStyle       = nullptr;

    std::atomic<float>* rawInputGain   = nullptr;
    std::atomic<float>* rawInputPan    = nullptr;
    std::atomic<float>* rawOutputGain  = nullptr;
    std::atomic<float>* rawOutputPan   = nullptr;
    std::atomic<float>* rawWetGain     = nullptr;
    std::atomic<float>* rawAutoGain    = nullptr;
    std::atomic<float>* rawMix         = nullptr;

    std::atomic<float>* rawSCExternal  = nullptr;
    std::atomic<float>* rawSCAudition  = nullptr;
    std::atomic<float>* rawSCLCFreq    = nullptr;
    std::atomic<float>* rawSCLCOn      = nullptr;
    std::atomic<float>* rawSCBandFreq  = nullptr;
    std::atomic<float>* rawSCBandGain  = nullptr;
    std::atomic<float>* rawSCHCFreq    = nullptr;
    std::atomic<float>* rawSCHCOn      = nullptr;

    std::atomic<float>* rawStereoLink  = nullptr;
    std::atomic<float>* rawStereoMode  = nullptr;

    std::atomic<float>* rawAnalyzerOn  = nullptr;
    std::atomic<float>* rawAnalyzerPre = nullptr;
    std::atomic<float>* rawOversample  = nullptr;
    std::atomic<float>* rawCompactMode = nullptr;
};
