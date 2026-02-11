#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "KneeDisplay.h"
#include "LevelMeterComponent.h"
#include "GainReductionDisplay.h"
#include "../../analyzer/SpectrumAnalyzer.h"
#include "../../analyzer/FFTProcessor.h"
#include "../../metering/LevelMeterData.h"
#include "../../metering/PeakHoldCalculator.h"
#include "../LookAndFeel/ColourPalette.h"
#include "../../utils/Constants.h"

// Forward declaration
class AdvancedCompressorProcessor;

class CompressorDisplay : public juce::Component,
                           public juce::Timer
{
public:
    explicit CompressorDisplay (AdvancedCompressorProcessor& processor);
    ~CompressorDisplay() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    AdvancedCompressorProcessor& processor;

    KneeDisplay kneeDisplay;
    LevelMeterComponent inputMeter;
    LevelMeterComponent outputMeter;
    GainReductionDisplay grDisplay;

    SpectrumAnalyzer spectrumPre;
    SpectrumAnalyzer spectrumPost;

    PeakHoldCalculator inputPeakL, inputPeakR;
    PeakHoldCalculator outputPeakL, outputPeakR;

    // Loudness readout
    juce::Label loudnessLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorDisplay)
};
