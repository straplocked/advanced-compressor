#include "CompressorEngine.h"
#include "../utils/MathUtils.h"

CompressorEngine::CompressorEngine() = default;

void CompressorEngine::prepare (double sampleRate, int maxBlockSize, int numChannels)
{
    currentSampleRate = sampleRate;
    currentBlockSize = maxBlockSize;

    for (auto& core : cores)
        core.prepare (sampleRate);
    for (auto& sc : sidechainEQ)
        sc.prepare (sampleRate);

    oversampler.prepare (sampleRate, maxBlockSize, numChannels);
    autoGainComputer.prepare (sampleRate);
    for (auto& cp : characterProcessors)
        cp.prepare (sampleRate);
    autoThreshold.prepare (sampleRate);
    midiTrigger.prepare (sampleRate);
    hostSyncPulse.prepare (sampleRate);

    // Allocate lookahead buffer (max 20ms)
    int maxLookaheadSamples = (int)(0.020 * sampleRate) + 1;
    lookaheadBuffer.resize ((size_t) numChannels);
    for (auto& ch : lookaheadBuffer)
        ch.assign ((size_t) maxLookaheadSamples, 0.0f);
    lookaheadWritePos = 0;
    lookaheadSamples = 0;

    dryBuffer.setSize (numChannels, maxBlockSize);
    scEQBuffer.setSize (numChannels, maxBlockSize);
    triggerBuffer.assign ((size_t) maxBlockSize, 0.0f);

    meterData.prepare (sampleRate);
}

void CompressorEngine::reset()
{
    for (auto& core : cores)
        core.reset();
    for (auto& sc : sidechainEQ)
        sc.reset();
    oversampler.reset();
    autoGainComputer.reset();
    for (auto& cp : characterProcessors)
        cp.reset();
    autoThreshold.reset();
    midiTrigger.reset();
    hostSyncPulse.reset();

    for (auto& ch : lookaheadBuffer)
        std::fill (ch.begin(), ch.end(), 0.0f);
    lookaheadWritePos = 0;

    meterData.reset();
}

void CompressorEngine::updateParameters (const CompressorParameters& params)
{
    cachedParams = params;

    auto style = static_cast<CompressorStyle> (params.style);
    auto behavior = getStyleBehavior (style);

    float effectiveKnee = params.knee + behavior.kneeOffset;
    float effectiveRatio = params.ratio * behavior.ratioScale;

    for (auto& core : cores)
    {
        core.setStyle (style);
        core.setThreshold (params.threshold);
        core.setRatio (effectiveRatio);
        core.setKnee (effectiveKnee);
        core.setRange (params.range);
        core.setAttack (params.attackMs);
        core.setRelease (params.releaseMs);
        core.setHold (params.holdMs);
    }

    for (auto& sc : sidechainEQ)
    {
        sc.setLowCut (params.scLCFreq, params.scLCOn, params.scLCSlope);
        sc.setLowShelf (params.scLSFreq, params.scLSGain, params.scLSOn);
        sc.setBand1 (params.scBandFreq, params.scBandGain, params.scBandQ);
        sc.setBand2 (params.scBand2Freq, params.scBand2Gain, params.scBand2Q);
        sc.setHighShelf (params.scHSFreq, params.scHSGain, params.scHSOn);
        sc.setHighCut (params.scHCFreq, params.scHCOn, params.scHCSlope);
    }

    oversampler.setFactor (params.oversample);
    lookaheadSamples = CompressorMath::msToSamples (params.lookaheadMs, currentSampleRate);

    // Character processing
    auto charMode = static_cast<CharacterMode> (params.charMode);
    for (auto& cp : characterProcessors)
    {
        cp.setMode (charMode);
        cp.setDrive (params.charDrive);
    }

    // Auto threshold
    autoThreshold.setEnabled (params.autoThresh);

    // Host sync
    hostSyncPulse.setDivision (params.syncDiv);
    hostSyncPulse.setShape (params.syncShape);
}

void CompressorEngine::process (juce::AudioBuffer<float>& buffer,
                                 const juce::AudioBuffer<float>* sidechainBuffer,
                                 const juce::MidiBuffer& midiMessages,
                                 juce::AudioPlayHead* playHead)
{
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    // Generate MIDI trigger / host sync envelopes
    bool useMIDITrigger = cachedParams.midiTrigger;
    bool useHostSync = cachedParams.hostSync;

    if (useMIDITrigger)
    {
        if ((int) triggerBuffer.size() < numSamples)
            triggerBuffer.resize ((size_t) numSamples);
        midiTrigger.processBlock (midiMessages, triggerBuffer.data(), numSamples);
    }
    else if (useHostSync)
    {
        if ((int) triggerBuffer.size() < numSamples)
            triggerBuffer.resize ((size_t) numSamples);
        hostSyncPulse.processBlock (playHead, triggerBuffer.data(), numSamples);
    }

    // Apply input gain + pan
    applyInputGain (buffer);
    if (numChannels >= 2)
        applyPan (buffer, cachedParams.inputPan);

    // Measure input levels
    meterData.pushInputLevel (buffer);

    // Feed auto threshold tracker
    float blockPeak = 0.0f;
    for (int ch = 0; ch < numChannels; ++ch)
    {
        const float* data = buffer.getReadPointer (ch);
        for (int i = 0; i < numSamples; ++i)
            blockPeak = std::max (blockPeak, std::abs (data[i]));
    }
    float blockPeakDB = blockPeak > 1e-10f ? 20.0f * std::log10 (blockPeak) : -80.0f;
    autoThreshold.pushLevel (blockPeakDB);

    // Apply auto threshold offset to cores
    float threshOffset = autoThreshold.getThresholdOffset();
    if (std::abs (threshOffset) > 0.01f)
    {
        float effectiveThresh = cachedParams.threshold + threshOffset;
        for (auto& core : cores)
            core.setThreshold (effectiveThresh);
    }

    // Save dry copy for mix
    dryBuffer.makeCopyOf (buffer, true);

    // Determine sidechain source
    const juce::AudioBuffer<float>& scSource =
        (cachedParams.scExternal && sidechainBuffer != nullptr) ? *sidechainBuffer : buffer;

    // M/S encoding if needed
    bool isMidMode = cachedParams.stereoMode == 1;
    bool isSideMode = cachedParams.stereoMode == 2;
    bool isMidToSide = cachedParams.stereoMode == 3;
    bool isSideToMid = cachedParams.stereoMode == 4;
    bool needMS = (isMidMode || isSideMode || isMidToSide || isSideToMid) && numChannels >= 2;

    if (needMS)
    {
        MidSideProcessor::encode (buffer.getWritePointer (0),
                                   buffer.getWritePointer (1),
                                   numSamples);
    }

    // Process compression per-sample
    float stereoLink = cachedParams.stereoLink / 100.0f;

    for (int i = 0; i < numSamples; ++i)
    {
        // Get sidechain samples per channel
        float scL, scR;

        if (useMIDITrigger || useHostSync)
        {
            // Use trigger envelope as sidechain signal (amplitude modulation)
            float trigLevel = triggerBuffer[(size_t) i];
            scL = trigLevel;
            scR = trigLevel;
        }
        else
        {
            scL = scSource.getReadPointer (0)[i];
            scR = (scSource.getNumChannels() >= 2) ? scSource.getReadPointer (1)[i] : scL;
        }

        // Apply sidechain EQ
        scL = sidechainEQ[0].processSample (scL);
        if (numChannels >= 2)
            scR = sidechainEQ[1].processSample (scR);

        // Store post-EQ sidechain for audition
        scEQBuffer.setSample (0, i, scL);
        if (numChannels >= 2)
            scEQBuffer.setSample (1, i, scR);

        // Compute GR per channel
        float grL = cores[0].processSidechain (scL);
        float grR = (numChannels >= 2) ? cores[1].processSidechain (scR) : grL;

        // Stereo linking
        if (numChannels >= 2 && stereoLink > 0.0f)
        {
            float linkedGR = std::min (grL, grR);
            grL = grL + stereoLink * (linkedGR - grL);
            grR = grR + stereoLink * (linkedGR - grR);
        }

        // M/S mode: only compress one component
        if (isMidMode) grR = 0.0f;
        if (isSideMode) grL = 0.0f;

        // Cross-linking: M>S = mid GR applied to side, S>M = side GR applied to mid
        if (isMidToSide) grR = grL;
        if (isSideToMid) grL = grR;

        // Apply gain reduction with lookahead
        float gainL = MathUtils::dbToGain (grL);
        float gainR = MathUtils::dbToGain (grR);

        if (lookaheadSamples > 0)
        {
            int maxLA = (int) lookaheadBuffer[0].size();
            int readPos = (lookaheadWritePos - lookaheadSamples + maxLA) % maxLA;

            for (int ch = 0; ch < numChannels; ++ch)
            {
                lookaheadBuffer[(size_t) ch][(size_t) lookaheadWritePos] = buffer.getSample (ch, i);
                float delayed = lookaheadBuffer[(size_t) ch][(size_t) readPos];
                float gain = (ch == 0) ? gainL : gainR;
                buffer.setSample (ch, i, delayed * gain);
            }
            lookaheadWritePos = (lookaheadWritePos + 1) % maxLA;
        }
        else
        {
            buffer.getWritePointer (0)[i] *= gainL;
            if (numChannels >= 2)
                buffer.getWritePointer (1)[i] *= gainR;
        }

        // Track auto gain
        autoGainComputer.pushGainReduction (std::min (grL, grR));
    }

    // M/S decode
    if (needMS)
    {
        MidSideProcessor::decode (buffer.getWritePointer (0),
                                   buffer.getWritePointer (1),
                                   numSamples);
    }

    // Push GR meter data
    meterData.pushGainReduction (cores[0].getCurrentGR(),
                                  numChannels >= 2 ? cores[1].getCurrentGR() : cores[0].getCurrentGR());

    // Character processing (saturation)
    if (cachedParams.charMode > 0)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            float* data = buffer.getWritePointer (ch);
            auto& charProc = characterProcessors[(size_t) ch];
            for (int i = 0; i < numSamples; ++i)
                data[i] = charProc.processSample (data[i]);
        }
    }

    // Wet gain
    float wetGainLinear = MathUtils::dbToGain (cachedParams.wetGain);
    buffer.applyGain (wetGainLinear);

    // Auto gain
    if (cachedParams.autoGain)
    {
        float autoGainLinear = MathUtils::dbToGain (autoGainComputer.getAutoGainDB());
        buffer.applyGain (autoGainLinear);
    }

    // Dry/Wet mix (0-200%)
    float mixAmount = cachedParams.mix / 100.0f;
    if (std::abs (mixAmount - 1.0f) > 0.001f)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            float* wet = buffer.getWritePointer (ch);
            const float* dry = dryBuffer.getReadPointer (ch);
            for (int i = 0; i < numSamples; ++i)
                wet[i] = dry[i] + mixAmount * (wet[i] - dry[i]);
        }
    }

    // Delta mode: output only the difference (what compression removed)
    if (cachedParams.delta)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            float* wet = buffer.getWritePointer (ch);
            const float* dry = dryBuffer.getReadPointer (ch);
            for (int i = 0; i < numSamples; ++i)
                wet[i] = dry[i] - wet[i];
        }
    }

    // SC audition: replace output with post-EQ sidechain signal,
    // modulated by GR to hear compression triggering intensity
    if (cachedParams.scAudition)
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            float* out = buffer.getWritePointer (ch);
            const float* scEQ = scEQBuffer.getReadPointer (std::min (ch, scEQBuffer.getNumChannels() - 1));
            for (int i = 0; i < numSamples; ++i)
                out[i] = scEQ[i];
        }
    }

    // Output gain + pan
    applyOutputGain (buffer);
    if (numChannels >= 2)
        applyPan (buffer, cachedParams.outputPan);

    // Measure output levels
    meterData.pushOutputLevel (buffer);
}

float CompressorEngine::getCurrentGR() const
{
    return std::min (cores[0].getCurrentGR(), cores[1].getCurrentGR());
}

int CompressorEngine::getLatencySamples() const
{
    return lookaheadSamples + oversampler.getLatencySamples();
}

void CompressorEngine::applyInputGain (juce::AudioBuffer<float>& buffer)
{
    if (std::abs (cachedParams.inputGain) > 0.01f)
    {
        float gain = MathUtils::dbToGain (cachedParams.inputGain);
        buffer.applyGain (gain);
    }
}

void CompressorEngine::applyOutputGain (juce::AudioBuffer<float>& buffer)
{
    if (std::abs (cachedParams.outputGain) > 0.01f)
    {
        float gain = MathUtils::dbToGain (cachedParams.outputGain);
        buffer.applyGain (gain);
    }
}

void CompressorEngine::applyPan (juce::AudioBuffer<float>& buffer, float pan)
{
    if (buffer.getNumChannels() < 2 || std::abs (pan) < 0.001f)
        return;

    float leftGain = std::cos ((pan + 1.0f) * 0.25f * juce::MathConstants<float>::pi);
    float rightGain = std::sin ((pan + 1.0f) * 0.25f * juce::MathConstants<float>::pi);

    buffer.applyGain (0, 0, buffer.getNumSamples(), leftGain);
    buffer.applyGain (1, 0, buffer.getNumSamples(), rightGain);
}
