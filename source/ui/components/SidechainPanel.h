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
    // Band 1: Low Cut
    juce::Slider lcFreqSlider;
    juce::TextButton lcOnButton { "LC" };
    juce::ComboBox lcSlopeBox;
    juce::Label lcLabel;

    // Band 2: Low Shelf
    juce::Slider lsFreqSlider, lsGainSlider;
    juce::TextButton lsOnButton { "LS" };
    juce::Label lsFreqLabel, lsGainLabel;

    // Band 3: Bell 1
    juce::Slider band1FreqSlider, band1GainSlider, band1QSlider;
    juce::Label band1FreqLabel, band1GainLabel, band1QLabel;

    // Band 4: Bell 2
    juce::Slider band2FreqSlider, band2GainSlider, band2QSlider;
    juce::Label band2FreqLabel, band2GainLabel, band2QLabel;

    // Band 5: High Shelf
    juce::Slider hsFreqSlider, hsGainSlider;
    juce::TextButton hsOnButton { "HS" };
    juce::Label hsFreqLabel, hsGainLabel;

    // Band 6: High Cut
    juce::Slider hcFreqSlider;
    juce::TextButton hcOnButton { "HC" };
    juce::ComboBox hcSlopeBox;
    juce::Label hcLabel;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lcFreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lcOnAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lcSlopeAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lsFreqAttach, lsGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lsOnAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band1FreqAttach, band1GainAttach, band1QAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> band2FreqAttach, band2GainAttach, band2QAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hsFreqAttach, hsGainAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> hsOnAttach;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hcFreqAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> hcOnAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> hcSlopeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidechainPanel)
};
