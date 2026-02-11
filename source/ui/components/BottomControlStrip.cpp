#include "BottomControlStrip.h"
#include "../../PluginProcessor.h"
#include "../LookAndFeel/ColourPalette.h"

BottomControlStrip::BottomControlStrip (AdvancedCompressorProcessor& p)
    : processor (p),
      sidechainPanel (p.getAPVTS())
{
    auto& apvts = processor.getAPVTS();

    auto setupKnob = [this] (juce::Slider& slider, juce::Label& label, const juce::String& text)
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

    setupKnob (thresholdSlider, thresholdLabel, "Threshold");
    setupKnob (ratioSlider, ratioLabel, "Ratio");
    setupKnob (kneeSlider, kneeLabel, "Knee");
    setupKnob (rangeSlider, rangeLabel, "Range");
    setupKnob (attackSlider, attackLabel, "Attack");
    setupKnob (releaseSlider, releaseLabel, "Release");
    setupKnob (holdSlider, holdLabel, "Hold");
    setupKnob (mixSlider, mixLabel, "Mix");
    setupKnob (wetGainSlider, wetGainLabel, "Wet Gain");
    setupKnob (stereoLinkSlider, stereoLinkLabel, "Link");

    stereoModeSelector.addItemList (ParamIDs::getStereoModeNames(), 1);
    stereoModeSelector.setJustificationType (juce::Justification::centred);
    stereoModeLabel.setText ("Mode", juce::dontSendNotification);
    stereoModeLabel.setJustificationType (juce::Justification::centred);
    stereoModeLabel.setFont (10.0f);
    stereoModeLabel.setColour (juce::Label::textColourId, ColourPalette::axisText());
    addAndMakeVisible (stereoModeSelector);
    addAndMakeVisible (stereoModeLabel);

    addAndMakeVisible (sidechainPanel);
    addAndMakeVisible (thresholdRing);

    // Attachments
    thresholdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kThreshold, thresholdSlider);
    ratioAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRatio, ratioSlider);
    kneeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kKnee, kneeSlider);
    rangeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRange, rangeSlider);
    attackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kAttack, attackSlider);
    releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRelease, releaseSlider);
    holdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kHold, holdSlider);
    mixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kMix, mixSlider);
    wetGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kWetGain, wetGainSlider);
    stereoLinkAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kStereoLink, stereoLinkSlider);
    stereoModeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kStereoMode, stereoModeSelector);
}

BottomControlStrip::~BottomControlStrip()
{
    thresholdAttach.reset(); ratioAttach.reset(); kneeAttach.reset(); rangeAttach.reset();
    attackAttach.reset(); releaseAttach.reset(); holdAttach.reset();
    mixAttach.reset(); wetGainAttach.reset(); stereoLinkAttach.reset(); stereoModeAttach.reset();
}

void BottomControlStrip::setGainReduction (float grDB)
{
    thresholdRing.setGainReduction (grDB);
}

void BottomControlStrip::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient grad (ColourPalette::panelTop(), 0, bounds.getY(),
                               ColourPalette::panelBottom(), 0, bounds.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRect (bounds);

    g.setColour (ColourPalette::bevelLight());
    g.drawLine (0.0f, bounds.getY() + 0.5f, bounds.getRight(), bounds.getY() + 0.5f, 1.0f);
    g.setColour (ColourPalette::bevelDark());
    g.drawLine (0.0f, bounds.getY() + 1.5f, bounds.getRight(), bounds.getY() + 1.5f, 1.0f);
}

void BottomControlStrip::resized()
{
    auto bounds = getLocalBounds().reduced (8, 4);

    int knobW = 60;
    int knobH = bounds.getHeight() - 16;

    auto layoutKnob = [&] (juce::Slider& slider, juce::Label& label)
    {
        auto area = bounds.removeFromLeft (knobW);
        auto knobArea = area.removeFromTop (knobH);
        slider.setBounds (knobArea);
        label.setBounds (area);
    };

    // Threshold with ring overlay
    auto threshArea = bounds.removeFromLeft (knobW);
    auto threshKnobArea = threshArea.removeFromTop (knobH);
    thresholdSlider.setBounds (threshKnobArea);
    thresholdRing.setBounds (threshKnobArea);
    thresholdLabel.setBounds (threshArea);

    layoutKnob (ratioSlider, ratioLabel);
    layoutKnob (kneeSlider, kneeLabel);
    layoutKnob (rangeSlider, rangeLabel);

    bounds.removeFromLeft (8);

    layoutKnob (attackSlider, attackLabel);
    layoutKnob (releaseSlider, releaseLabel);
    layoutKnob (holdSlider, holdLabel);

    bounds.removeFromLeft (12);

    layoutKnob (mixSlider, mixLabel);
    layoutKnob (wetGainSlider, wetGainLabel);
    layoutKnob (stereoLinkSlider, stereoLinkLabel);

    auto modeArea = bounds.removeFromLeft (60);
    stereoModeSelector.setBounds (modeArea.removeFromTop (knobH).reduced (0, knobH / 3));
    stereoModeLabel.setBounds (modeArea);

    bounds.removeFromLeft (8);

    // Sidechain panel takes remaining space
    sidechainPanel.setBounds (bounds);
}
