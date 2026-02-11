#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "SidechainPanel.h"
#include "SidechainMeterRing.h"
#include "../../parameters/ParameterIDs.h"

class AdvancedCompressorProcessor;

class BottomControlStrip : public juce::Component
{
public:
    explicit BottomControlStrip (AdvancedCompressorProcessor& processor);
    ~BottomControlStrip() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void setGainReduction (float grDB);

private:
    AdvancedCompressorProcessor& processor;

    // Main compressor knobs
    juce::Slider thresholdSlider, ratioSlider, kneeSlider, rangeSlider;
    juce::Slider attackSlider, releaseSlider, holdSlider;
    juce::Slider mixSlider, wetGainSlider, stereoLinkSlider;
    juce::ComboBox stereoModeSelector;

    juce::Label thresholdLabel, ratioLabel, kneeLabel, rangeLabel;
    juce::Label attackLabel, releaseLabel, holdLabel;
    juce::Label mixLabel, wetGainLabel, stereoLinkLabel, stereoModeLabel;

    SidechainPanel sidechainPanel;
    SidechainMeterRing thresholdRing;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kneeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> holdAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stereoLinkAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> stereoModeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomControlStrip)
};
