#include "SidechainPanel.h"
#include "../LookAndFeel/ColourPalette.h"

SidechainPanel::SidechainPanel (juce::AudioProcessorValueTreeState& apvts)
{
    auto setupSlider = [this] (juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 14);
        addAndMakeVisible (slider);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (9.0f);
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    auto setupToggle = [this] (juce::TextButton& btn)
    {
        btn.setClickingTogglesState (true);
        btn.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.3f));
        addAndMakeVisible (btn);
    };

    auto setupCombo = [this] (juce::ComboBox& box)
    {
        box.addItemList (ParamIDs::getSCSlopeNames(), 1);
        box.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (box);
    };

    // Band 1: Low Cut
    setupSlider (lcFreqSlider, lcLabel, "Freq");
    setupToggle (lcOnButton);
    setupCombo (lcSlopeBox);

    // Band 2: Low Shelf
    setupSlider (lsFreqSlider, lsFreqLabel, "Freq");
    setupSlider (lsGainSlider, lsGainLabel, "Gain");
    setupToggle (lsOnButton);

    // Band 3: Bell 1
    setupSlider (band1FreqSlider, band1FreqLabel, "Freq");
    setupSlider (band1GainSlider, band1GainLabel, "Gain");
    setupSlider (band1QSlider, band1QLabel, "Q");

    // Band 4: Bell 2
    setupSlider (band2FreqSlider, band2FreqLabel, "Freq");
    setupSlider (band2GainSlider, band2GainLabel, "Gain");
    setupSlider (band2QSlider, band2QLabel, "Q");

    // Band 5: High Shelf
    setupSlider (hsFreqSlider, hsFreqLabel, "Freq");
    setupSlider (hsGainSlider, hsGainLabel, "Gain");
    setupToggle (hsOnButton);

    // Band 6: High Cut
    setupSlider (hcFreqSlider, hcLabel, "Freq");
    setupToggle (hcOnButton);
    setupCombo (hcSlopeBox);

    // Attachments
    lcFreqAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCLCFreq, lcFreqSlider);
    lcOnAttach    = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kSCLCOn, lcOnButton);
    lcSlopeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kSCLCSlope, lcSlopeBox);

    lsFreqAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCLSFreq, lsFreqSlider);
    lsGainAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCLSGain, lsGainSlider);
    lsOnAttach    = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kSCLSOn, lsOnButton);

    band1FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBandFreq, band1FreqSlider);
    band1GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBandGain, band1GainSlider);
    band1QAttach    = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBandQ, band1QSlider);

    band2FreqAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBand2Freq, band2FreqSlider);
    band2GainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBand2Gain, band2GainSlider);
    band2QAttach    = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCBand2Q, band2QSlider);

    hsFreqAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCHSFreq, hsFreqSlider);
    hsGainAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCHSGain, hsGainSlider);
    hsOnAttach    = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kSCHSOn, hsOnButton);

    hcFreqAttach  = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kSCHCFreq, hcFreqSlider);
    hcOnAttach    = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kSCHCOn, hcOnButton);
    hcSlopeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kSCHCSlope, hcSlopeBox);
}

SidechainPanel::~SidechainPanel()
{
    lcFreqAttach.reset();
    lcOnAttach.reset();
    lcSlopeAttach.reset();
    lsFreqAttach.reset();
    lsGainAttach.reset();
    lsOnAttach.reset();
    band1FreqAttach.reset();
    band1GainAttach.reset();
    band1QAttach.reset();
    band2FreqAttach.reset();
    band2GainAttach.reset();
    band2QAttach.reset();
    hsFreqAttach.reset();
    hsGainAttach.reset();
    hsOnAttach.reset();
    hcFreqAttach.reset();
    hcOnAttach.reset();
    hcSlopeAttach.reset();
}

void SidechainPanel::paint (juce::Graphics& g)
{
    g.setColour (ColourPalette::gridLine().withAlpha (0.3f));
    g.drawVerticalLine (0, 0.0f, (float) getHeight());

    g.setFont (10.0f);
    g.setColour (ColourPalette::axisText());
    g.drawText ("SC EQ (6-Band)", getLocalBounds().removeFromTop (14), juce::Justification::centred);
}

void SidechainPanel::resized()
{
    auto bounds = getLocalBounds().reduced (2);
    bounds.removeFromTop (16);

    int knobSize = 36;
    int labelH = 11;
    int btnH = 14;
    int slopeH = 16;
    int colW = 38;

    // Row 1: LC, LS, Band1 (top half)
    auto row1 = bounds.removeFromTop (knobSize + labelH + btnH + 2);

    // Low Cut column
    auto lcCol = row1.removeFromLeft (colW);
    lcOnButton.setBounds (lcCol.removeFromTop (btnH).withSizeKeepingCentre (30, btnH));
    lcFreqSlider.setBounds (lcCol.removeFromTop (knobSize));
    lcLabel.setBounds (lcCol.removeFromTop (labelH));
    row1.removeFromLeft (1);

    // LC slope (narrow dropdown)
    auto lcSlopeCol = row1.removeFromLeft (colW + 8);
    lcSlopeCol.removeFromTop (btnH);
    lcSlopeBox.setBounds (lcSlopeCol.removeFromTop (slopeH).withSizeKeepingCentre (44, slopeH));
    row1.removeFromLeft (2);

    // Low Shelf columns
    auto lsCol1 = row1.removeFromLeft (colW);
    lsOnButton.setBounds (lsCol1.removeFromTop (btnH).withSizeKeepingCentre (30, btnH));
    lsFreqSlider.setBounds (lsCol1.removeFromTop (knobSize));
    lsFreqLabel.setBounds (lsCol1.removeFromTop (labelH));
    row1.removeFromLeft (1);

    auto lsCol2 = row1.removeFromLeft (colW);
    lsCol2.removeFromTop (btnH);
    lsGainSlider.setBounds (lsCol2.removeFromTop (knobSize));
    lsGainLabel.setBounds (lsCol2.removeFromTop (labelH));
    row1.removeFromLeft (2);

    // Band 1 columns
    auto b1Col1 = row1.removeFromLeft (colW);
    b1Col1.removeFromTop (btnH);
    band1FreqSlider.setBounds (b1Col1.removeFromTop (knobSize));
    band1FreqLabel.setBounds (b1Col1.removeFromTop (labelH));
    row1.removeFromLeft (1);

    auto b1Col2 = row1.removeFromLeft (colW);
    b1Col2.removeFromTop (btnH);
    band1GainSlider.setBounds (b1Col2.removeFromTop (knobSize));
    band1GainLabel.setBounds (b1Col2.removeFromTop (labelH));
    row1.removeFromLeft (1);

    auto b1Col3 = row1.removeFromLeft (colW);
    b1Col3.removeFromTop (btnH);
    band1QSlider.setBounds (b1Col3.removeFromTop (knobSize));
    band1QLabel.setBounds (b1Col3.removeFromTop (labelH));

    bounds.removeFromTop (4);

    // Row 2: Band2, HS, HC (bottom half)
    auto row2 = bounds.removeFromTop (knobSize + labelH + btnH + 2);

    // Band 2 columns
    auto b2Col1 = row2.removeFromLeft (colW);
    b2Col1.removeFromTop (btnH);
    band2FreqSlider.setBounds (b2Col1.removeFromTop (knobSize));
    band2FreqLabel.setBounds (b2Col1.removeFromTop (labelH));
    row2.removeFromLeft (1);

    auto b2Col2 = row2.removeFromLeft (colW);
    b2Col2.removeFromTop (btnH);
    band2GainSlider.setBounds (b2Col2.removeFromTop (knobSize));
    band2GainLabel.setBounds (b2Col2.removeFromTop (labelH));
    row2.removeFromLeft (1);

    auto b2Col3 = row2.removeFromLeft (colW);
    b2Col3.removeFromTop (btnH);
    band2QSlider.setBounds (b2Col3.removeFromTop (knobSize));
    band2QLabel.setBounds (b2Col3.removeFromTop (labelH));
    row2.removeFromLeft (2);

    // High Shelf columns
    auto hsCol1 = row2.removeFromLeft (colW);
    hsOnButton.setBounds (hsCol1.removeFromTop (btnH).withSizeKeepingCentre (30, btnH));
    hsFreqSlider.setBounds (hsCol1.removeFromTop (knobSize));
    hsFreqLabel.setBounds (hsCol1.removeFromTop (labelH));
    row2.removeFromLeft (1);

    auto hsCol2 = row2.removeFromLeft (colW);
    hsCol2.removeFromTop (btnH);
    hsGainSlider.setBounds (hsCol2.removeFromTop (knobSize));
    hsGainLabel.setBounds (hsCol2.removeFromTop (labelH));
    row2.removeFromLeft (2);

    // High Cut column
    auto hcCol = row2.removeFromLeft (colW);
    hcOnButton.setBounds (hcCol.removeFromTop (btnH).withSizeKeepingCentre (30, btnH));
    hcFreqSlider.setBounds (hcCol.removeFromTop (knobSize));
    hcLabel.setBounds (hcCol.removeFromTop (labelH));
    row2.removeFromLeft (1);

    // HC slope
    auto hcSlopeCol = row2.removeFromLeft (colW + 8);
    hcSlopeCol.removeFromTop (btnH);
    hcSlopeBox.setBounds (hcSlopeCol.removeFromTop (slopeH).withSizeKeepingCentre (44, slopeH));
}
