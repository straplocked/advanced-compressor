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
    bool  delta       = false;
    int   charMode    = 0;
    float charDrive   = 0.0f;
    bool  autoThresh  = false;
    float scLCFreq    = 20.0f;
    bool  scLCOn      = false;
    int   scLCSlope   = 1;     // 0=6, 1=12, 2=24, 3=48, 4=96 dB/oct
    float scLSFreq    = 200.0f;
    float scLSGain    = 0.0f;
    bool  scLSOn      = false;
    float scBandFreq  = 1000.0f;
    float scBandGain  = 0.0f;
    float scBandQ     = 1.0f;
    float scBand2Freq = 3000.0f;
    float scBand2Gain = 0.0f;
    float scBand2Q    = 1.0f;
    float scHSFreq    = 8000.0f;
    float scHSGain    = 0.0f;
    bool  scHSOn      = false;
    float scHCFreq    = 20000.0f;
    bool  scHCOn      = false;
    int   scHCSlope   = 1;

    float stereoLink  = 100.0f;
    int   stereoMode  = 0;

    bool  midiTrigger = false;
    bool  hostSync    = false;
    int   syncDiv     = 2;   // default 1/4
    int   syncShape   = 0;   // default Square

    bool  analyzerOn  = true;
    bool  analyzerPre = false;
    int   oversample  = 0;
    bool  compactMode = false;
    int   meterScale  = 3;  // default 60 dB
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
        rawDelta       = apvts.getRawParameterValue (ParamIDs::kDelta);
        rawCharMode    = apvts.getRawParameterValue (ParamIDs::kCharMode);
        rawCharDrive   = apvts.getRawParameterValue (ParamIDs::kCharDrive);
        rawAutoThresh  = apvts.getRawParameterValue (ParamIDs::kAutoThresh);
        rawSCLCFreq    = apvts.getRawParameterValue (ParamIDs::kSCLCFreq);
        rawSCLCOn      = apvts.getRawParameterValue (ParamIDs::kSCLCOn);
        rawSCLCSlope   = apvts.getRawParameterValue (ParamIDs::kSCLCSlope);
        rawSCLSFreq    = apvts.getRawParameterValue (ParamIDs::kSCLSFreq);
        rawSCLSGain    = apvts.getRawParameterValue (ParamIDs::kSCLSGain);
        rawSCLSOn      = apvts.getRawParameterValue (ParamIDs::kSCLSOn);
        rawSCBandFreq  = apvts.getRawParameterValue (ParamIDs::kSCBandFreq);
        rawSCBandGain  = apvts.getRawParameterValue (ParamIDs::kSCBandGain);
        rawSCBandQ     = apvts.getRawParameterValue (ParamIDs::kSCBandQ);
        rawSCBand2Freq = apvts.getRawParameterValue (ParamIDs::kSCBand2Freq);
        rawSCBand2Gain = apvts.getRawParameterValue (ParamIDs::kSCBand2Gain);
        rawSCBand2Q    = apvts.getRawParameterValue (ParamIDs::kSCBand2Q);
        rawSCHSFreq    = apvts.getRawParameterValue (ParamIDs::kSCHSFreq);
        rawSCHSGain    = apvts.getRawParameterValue (ParamIDs::kSCHSGain);
        rawSCHSOn      = apvts.getRawParameterValue (ParamIDs::kSCHSOn);
        rawSCHCFreq    = apvts.getRawParameterValue (ParamIDs::kSCHCFreq);
        rawSCHCOn      = apvts.getRawParameterValue (ParamIDs::kSCHCOn);
        rawSCHCSlope   = apvts.getRawParameterValue (ParamIDs::kSCHCSlope);

        rawStereoLink  = apvts.getRawParameterValue (ParamIDs::kStereoLink);
        rawStereoMode  = apvts.getRawParameterValue (ParamIDs::kStereoMode);

        rawMIDITrigger = apvts.getRawParameterValue (ParamIDs::kMIDITrigger);
        rawHostSync    = apvts.getRawParameterValue (ParamIDs::kHostSync);
        rawSyncDiv     = apvts.getRawParameterValue (ParamIDs::kSyncDiv);
        rawSyncShape   = apvts.getRawParameterValue (ParamIDs::kSyncShape);

        rawAnalyzerOn  = apvts.getRawParameterValue (ParamIDs::kAnalyzerOn);
        rawAnalyzerPre = apvts.getRawParameterValue (ParamIDs::kAnalyzerPre);
        rawOversample  = apvts.getRawParameterValue (ParamIDs::kOversample);
        rawCompactMode = apvts.getRawParameterValue (ParamIDs::kCompactMode);
        rawMeterScale  = apvts.getRawParameterValue (ParamIDs::kMeterScale);
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
        p.delta       = rawDelta->load() >= 0.5f;
        p.charMode    = (int) rawCharMode->load();
        p.charDrive   = rawCharDrive->load();
        p.autoThresh  = rawAutoThresh->load() >= 0.5f;
        p.scLCFreq    = rawSCLCFreq->load();
        p.scLCOn      = rawSCLCOn->load() >= 0.5f;
        p.scLCSlope   = (int) rawSCLCSlope->load();
        p.scLSFreq    = rawSCLSFreq->load();
        p.scLSGain    = rawSCLSGain->load();
        p.scLSOn      = rawSCLSOn->load() >= 0.5f;
        p.scBandFreq  = rawSCBandFreq->load();
        p.scBandGain  = rawSCBandGain->load();
        p.scBandQ     = rawSCBandQ->load();
        p.scBand2Freq = rawSCBand2Freq->load();
        p.scBand2Gain = rawSCBand2Gain->load();
        p.scBand2Q    = rawSCBand2Q->load();
        p.scHSFreq    = rawSCHSFreq->load();
        p.scHSGain    = rawSCHSGain->load();
        p.scHSOn      = rawSCHSOn->load() >= 0.5f;
        p.scHCFreq    = rawSCHCFreq->load();
        p.scHCOn      = rawSCHCOn->load() >= 0.5f;
        p.scHCSlope   = (int) rawSCHCSlope->load();

        p.stereoLink  = rawStereoLink->load();
        p.stereoMode  = (int) rawStereoMode->load();

        p.midiTrigger = rawMIDITrigger->load() >= 0.5f;
        p.hostSync    = rawHostSync->load() >= 0.5f;
        p.syncDiv     = (int) rawSyncDiv->load();
        p.syncShape   = (int) rawSyncShape->load();

        p.analyzerOn  = rawAnalyzerOn->load() >= 0.5f;
        p.analyzerPre = rawAnalyzerPre->load() >= 0.5f;
        p.oversample  = (int) rawOversample->load();
        p.compactMode = rawCompactMode->load() >= 0.5f;
        p.meterScale  = (int) rawMeterScale->load();
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
    std::atomic<float>* rawDelta       = nullptr;
    std::atomic<float>* rawCharMode    = nullptr;
    std::atomic<float>* rawCharDrive   = nullptr;
    std::atomic<float>* rawAutoThresh  = nullptr;
    std::atomic<float>* rawSCLCFreq    = nullptr;
    std::atomic<float>* rawSCLCOn      = nullptr;
    std::atomic<float>* rawSCLCSlope   = nullptr;
    std::atomic<float>* rawSCLSFreq    = nullptr;
    std::atomic<float>* rawSCLSGain    = nullptr;
    std::atomic<float>* rawSCLSOn      = nullptr;
    std::atomic<float>* rawSCBandFreq  = nullptr;
    std::atomic<float>* rawSCBandGain  = nullptr;
    std::atomic<float>* rawSCBandQ     = nullptr;
    std::atomic<float>* rawSCBand2Freq = nullptr;
    std::atomic<float>* rawSCBand2Gain = nullptr;
    std::atomic<float>* rawSCBand2Q    = nullptr;
    std::atomic<float>* rawSCHSFreq    = nullptr;
    std::atomic<float>* rawSCHSGain    = nullptr;
    std::atomic<float>* rawSCHSOn      = nullptr;
    std::atomic<float>* rawSCHCFreq    = nullptr;
    std::atomic<float>* rawSCHCOn      = nullptr;
    std::atomic<float>* rawSCHCSlope   = nullptr;

    std::atomic<float>* rawStereoLink  = nullptr;
    std::atomic<float>* rawStereoMode  = nullptr;

    std::atomic<float>* rawMIDITrigger = nullptr;
    std::atomic<float>* rawHostSync    = nullptr;
    std::atomic<float>* rawSyncDiv     = nullptr;
    std::atomic<float>* rawSyncShape   = nullptr;

    std::atomic<float>* rawAnalyzerOn  = nullptr;
    std::atomic<float>* rawAnalyzerPre = nullptr;
    std::atomic<float>* rawOversample  = nullptr;
    std::atomic<float>* rawCompactMode = nullptr;
    std::atomic<float>* rawMeterScale  = nullptr;
};
