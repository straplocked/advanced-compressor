#include "CompressorCore.h"

void CompressorCore::prepare (double sampleRate)
{
    detector.prepare (sampleRate);
    envelope.prepare (sampleRate);
    upwardEnvelope.prepare (sampleRate);
    reset();
}

void CompressorCore::reset()
{
    detector.reset();
    envelope.reset();
    upwardEnvelope.reset();
    currentGR = 0.0f;
    feedbackGR = 0.0f;
}

void CompressorCore::setStyle (CompressorStyle style)
{
    auto behavior = getStyleBehavior (style);
    detector.setMode (behavior.usePeak, behavior.useRMS);
    envelope.setStyle (style);
    gainComputer.setVariableRatio (behavior.variableRatio);
    feedbackMode = behavior.feedback;

    upwardMode = behavior.upward;
    ttmMode = behavior.ttm;

    // Configure upward gain computer mode
    gainComputer.setUpwardMode (upwardMode);

    if (ttmMode)
    {
        // TTM: main gain computer does downward, second does upward
        gainComputer.setUpwardMode (false);
        upwardGainComputer.setUpwardMode (true);
        upwardEnvelope.setStyle (style);
    }
}

void CompressorCore::setThreshold (float dB)
{
    gainComputer.setParameters (dB, gainComputer.getRatio(), gainComputer.getKnee(), 60.0f);
    if (ttmMode)
        upwardGainComputer.setParameters (dB, upwardGainComputer.getRatio(), upwardGainComputer.getKnee(), 60.0f);
}

void CompressorCore::setRatio (float ratio)
{
    gainComputer.setParameters (gainComputer.getThreshold(), ratio, gainComputer.getKnee(), 60.0f);
    if (ttmMode)
        upwardGainComputer.setParameters (upwardGainComputer.getThreshold(), ratio, upwardGainComputer.getKnee(), 60.0f);
}

void CompressorCore::setKnee (float dB)
{
    gainComputer.setParameters (gainComputer.getThreshold(), gainComputer.getRatio(), dB, 60.0f);
    if (ttmMode)
        upwardGainComputer.setParameters (upwardGainComputer.getThreshold(), upwardGainComputer.getRatio(), dB, 60.0f);
}

void CompressorCore::setRange (float dB)
{
    gainComputer.setParameters (gainComputer.getThreshold(), gainComputer.getRatio(), gainComputer.getKnee(), dB);
    if (ttmMode)
        upwardGainComputer.setParameters (upwardGainComputer.getThreshold(), upwardGainComputer.getRatio(), upwardGainComputer.getKnee(), dB);
}

void CompressorCore::setAttack (float ms)
{
    envelope.setAttack (ms);
    if (ttmMode)
        upwardEnvelope.setAttack (ms);
}

void CompressorCore::setRelease (float ms)
{
    envelope.setRelease (ms);
    if (ttmMode)
        upwardEnvelope.setRelease (ms);
}

void CompressorCore::setHold (float ms)
{
    envelope.setHold (ms);
    if (ttmMode)
        upwardEnvelope.setHold (ms);
}

float CompressorCore::processSidechain (float scSample)
{
    // In feedback mode, add previous gain reduction to the input
    float detectInput = scSample;
    if (feedbackMode)
    {
        float fbGainLinear = std::pow (10.0f, feedbackGR / 20.0f);
        detectInput = scSample * fbGainLinear;
    }

    float levelDB = detector.processSample (detectInput);

    if (ttmMode)
    {
        // TTM: compute both downward and upward, each with its own envelope
        float targetDownGR = gainComputer.computeGainReduction (levelDB);   // negative
        float targetUpGain = upwardGainComputer.computeGainReduction (levelDB);  // positive

        // Downward envelope (already expects negative values)
        float envDown = envelope.processSample (targetDownGR);

        // Upward envelope: negate so envelope sees "negative" attack/release semantics
        float envUp = -upwardEnvelope.processSample (-targetUpGain);

        // Combine: downward (negative) + upward (positive)
        currentGR = envDown + envUp;
    }
    else if (upwardMode)
    {
        // Upward: gain computer returns positive boost
        float targetBoost = gainComputer.computeGainReduction (levelDB);  // positive

        // Negate for envelope (envelope expects negative = more compression)
        float envResult = envelope.processSample (-targetBoost);

        // Negate back to positive boost
        currentGR = -envResult;
    }
    else
    {
        // Standard downward compression
        float targetGR = gainComputer.computeGainReduction (levelDB);
        currentGR = envelope.processSample (targetGR);
    }

    feedbackGR = currentGR;
    return currentGR;
}
