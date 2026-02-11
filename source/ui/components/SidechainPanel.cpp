#include "SidechainPanel.h"
#include "../LookAndFeel/ColourPalette.h"

SidechainPanel::SidechainPanel (juce::AudioProcessorValueTreeState& apvts)
{
    auto setupSlider = [this] (juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 55, 14);
        addAndMakeVisible (slider);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (10.0f);
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    setupSlider (lcFreqSlider, lcLabel, "LC Freq");
    setupSlider (bandFreqSlider, bandFreqLabel, "Band");
    setupSlider (bandGainSlider, bandGainLabel, "Gain");
    setupSlider (hcFreqSlider, hcLabel, "HC Freq");

    lcOnButton.setClickingTogglesState (true);
    lcOnButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.3f));
    addAndMakeVisible (lcOnButton);

    hcOnButton.setClickingTogglesState (true);
    hcOnButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.3f));
    addAndMakeVisible (hcOnButton);

    lcFreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCLCFreq, lcFreqSlider);
    bandFreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBandFreq, bandFreqSlider);
    bandGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBandGain, bandGainSlider);
    hcFreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCHCFreq, hcFreqSlider);
    lcOnAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kSCLCOn, lcOnButton);
    hcOnAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kSCHCOn, hcOnButton);
}

SidechainPanel::~SidechainPanel()
{
    lcFreqAttach.reset();
    bandFreqAttach.reset();
    bandGainAttach.reset();
    hcFreqAttach.reset();
    lcOnAttach.reset();
    hcOnAttach.reset();
}

void SidechainPanel::paint (juce::Graphics& g)
{
    g.setColour (ColourPalette::gridLine().withAlpha (0.3f));
    g.drawVerticalLine (0, 0.0f, (float) getHeight());

    g.setFont (10.0f);
    g.setColour (ColourPalette::axisText());
    g.drawText ("SC EQ", getLocalBounds().removeFromTop (14), juce::Justification::centred);
}

void SidechainPanel::resized()
{
    auto bounds = getLocalBounds().reduced (4);
    bounds.removeFromTop (16);

    int knobSize = 42;
    int spacing = 4;

    auto row = bounds.removeFromTop (knobSize + 16);
    auto lcArea = row.removeFromLeft (knobSize + spacing);
    lcOnButton.setBounds (lcArea.removeFromTop (16).withSizeKeepingCentre (30, 14));
    lcFreqSlider.setBounds (lcArea.removeFromTop (knobSize));
    lcLabel.setBounds (lcArea.removeFromTop (12));

    auto bandArea = row.removeFromLeft (knobSize + spacing);
    bandFreqSlider.setBounds (bandArea.removeFromTop (knobSize));
    bandFreqLabel.setBounds (bandArea.removeFromTop (12));

    auto gainArea = row.removeFromLeft (knobSize + spacing);
    bandGainSlider.setBounds (gainArea.removeFromTop (knobSize));
    bandGainLabel.setBounds (gainArea.removeFromTop (12));

    auto hcArea = row.removeFromLeft (knobSize + spacing);
    hcOnButton.setBounds (hcArea.removeFromTop (16).withSizeKeepingCentre (30, 14));
    hcFreqSlider.setBounds (hcArea.removeFromTop (knobSize));
    hcLabel.setBounds (hcArea.removeFromTop (12));
}
