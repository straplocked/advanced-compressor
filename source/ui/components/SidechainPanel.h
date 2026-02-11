#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../../parameters/ParameterIDs.h"

class SidechainPanel : public juce::Component
{
public:
    explicit SidechainPanel (juce::AudioProcessorValueTreeState& apvts);
    ~SidechainPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Slider lcFreqSlider, bandFreqSlider, bandGainSlider, hcFreqSlider;
    juce::TextButton lcOnButton { "LC" }, hcOnButton { "HC" };
    juce::Label lcLabel, bandFreqLabel, bandGainLabel, hcLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lcFreqAttach, bandFreqAttach, bandGainAttach, hcFreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lcOnAttach, hcOnAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidechainPanel)
};
