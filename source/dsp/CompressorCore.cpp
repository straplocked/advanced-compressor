#include "CompressorCore.h"

void CompressorCore::prepare (double sampleRate)
{
    detector.prepare (sampleRate);
    envelope.prepare (sampleRate);
    reset();
}

void CompressorCore::reset()
{
    detector.reset();
    envelope.reset();
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
}

void CompressorCore::setThreshold (float dB)
{
    gainComputer.setParameters (dB, gainComputer.getRatio(), gainComputer.getKnee(), 60.0f);
}

void CompressorCore::setRatio (float ratio)
{
    gainComputer.setParameters (gainComputer.getThreshold(), ratio, gainComputer.getKnee(), 60.0f);
}

void CompressorCore::setKnee (float dB)
{
    gainComputer.setParameters (gainComputer.getThreshold(), gainComputer.getRatio(), dB, 60.0f);
}

void CompressorCore::setRange (float dB)
{
    gainComputer.setParameters (gainComputer.getThreshold(), gainComputer.getRatio(), gainComputer.getKnee(), dB);
}

void CompressorCore::setAttack (float ms) { envelope.setAttack (ms); }
void CompressorCore::setRelease (float ms) { envelope.setRelease (ms); }
void CompressorCore::setHold (float ms) { envelope.setHold (ms); }

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
    float targetGR = gainComputer.computeGainReduction (levelDB);
    currentGR = envelope.processSample (targetGR);
    feedbackGR = currentGR;
    return currentGR;
}
