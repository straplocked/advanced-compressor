/*
 * Advanced Compressor - Open-source dynamic range compressor plugin
 * Copyright (C) 2024-2026 Advanced Compressor contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "SidechainPanel.h"
#include "../LookAndFeel/ColourPalette.h"
#include "../LookAndFeel/Fonts.h"

SidechainPanel::SidechainPanel (juce::AudioProcessorValueTreeState& apvts)
    : eqDisplay (apvts)
{
    addAndMakeVisible (eqDisplay);
    auto setupSlider = [this] (juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 13);
        addAndMakeVisible (slider);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (Fonts::getBody (10.0f));
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
    setupSlider (lcFreqSlider, lcFreqLabel, "Freq");
    setupToggle (lcOnButton);
    setupCombo (lcSlopeBox);
    lcOnButton.setComponentID ("sc.lcOn");
    lcFreqSlider.setComponentID ("sc.lcFreq");
    lcSlopeBox.setComponentID ("sc.lcSlope");

    // Band 2: Low Shelf
    setupSlider (lsFreqSlider, lsFreqLabel, "Freq");
    setupSlider (lsGainSlider, lsGainLabel, "Gain");
    setupToggle (lsOnButton);
    lsOnButton.setComponentID ("sc.lsOn");
    lsFreqSlider.setComponentID ("sc.lsFreq");
    lsGainSlider.setComponentID ("sc.lsGain");

    // Band 3: Bell 1
    setupSlider (band1FreqSlider, band1FreqLabel, "Freq");
    setupSlider (band1GainSlider, band1GainLabel, "Gain");
    setupSlider (band1QSlider, band1QLabel, "Q");
    band1FreqSlider.setComponentID ("sc.b1Freq");
    band1GainSlider.setComponentID ("sc.b1Gain");
    band1QSlider.setComponentID ("sc.b1Q");

    // Band 4: Bell 2
    setupSlider (band2FreqSlider, band2FreqLabel, "Freq");
    setupSlider (band2GainSlider, band2GainLabel, "Gain");
    setupSlider (band2QSlider, band2QLabel, "Q");
    band2FreqSlider.setComponentID ("sc.b2Freq");
    band2GainSlider.setComponentID ("sc.b2Gain");
    band2QSlider.setComponentID ("sc.b2Q");

    // Band 5: High Shelf
    setupSlider (hsFreqSlider, hsFreqLabel, "Freq");
    setupSlider (hsGainSlider, hsGainLabel, "Gain");
    setupToggle (hsOnButton);
    hsOnButton.setComponentID ("sc.hsOn");
    hsFreqSlider.setComponentID ("sc.hsFreq");
    hsGainSlider.setComponentID ("sc.hsGain");

    // Band 6: High Cut
    setupSlider (hcFreqSlider, hcFreqLabel, "Freq");
    setupToggle (hcOnButton);
    setupCombo (hcSlopeBox);
    hcOnButton.setComponentID ("sc.hcOn");
    hcFreqSlider.setComponentID ("sc.hcFreq");
    hcSlopeBox.setComponentID ("sc.hcSlope");

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
    lcFreqAttach.reset(); lcOnAttach.reset(); lcSlopeAttach.reset();
    lsFreqAttach.reset(); lsGainAttach.reset(); lsOnAttach.reset();
    band1FreqAttach.reset(); band1GainAttach.reset(); band1QAttach.reset();
    band2FreqAttach.reset(); band2GainAttach.reset(); band2QAttach.reset();
    hsFreqAttach.reset(); hsGainAttach.reset(); hsOnAttach.reset();
    hcFreqAttach.reset(); hcOnAttach.reset(); hcSlopeAttach.reset();
}

void SidechainPanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Top bevel
    g.setColour (ColourPalette::bevelLight().withAlpha (0.3f));
    g.drawLine (0.0f, bounds.getY() + 0.5f, bounds.getRight(), bounds.getY() + 0.5f, 1.0f);

    // Band separator lines
    int numBands = 6;
    float bandW = bounds.getWidth() / (float) numBands;
    g.setColour (ColourPalette::gridLine().withAlpha (0.4f));
    for (int i = 1; i < numBands; ++i)
    {
        float x = bandW * (float) i;
        g.drawLine (x, bounds.getY() + 4.0f, x, bounds.getBottom() - 4.0f, 0.5f);
    }

    // Band title labels
    g.setFont (Fonts::getBold (10.0f));
    g.setColour (ColourPalette::axisText());
    const char* titles[] = { "LC", "LS", "BELL 1", "BELL 2", "HS", "HC" };
    for (int i = 0; i < numBands; ++i)
    {
        auto labelBounds = juce::Rectangle<float> (bandW * (float) i, 2.0f, bandW, 12.0f);
        g.drawText (titles[i], labelBounds, juce::Justification::centred);
    }
}

void SidechainPanel::resized()
{
    auto bounds = getLocalBounds();
    eqDisplay.setBounds (bounds);
    int numBands = 6;
    int bandW = bounds.getWidth() / numBands;
    int knobSize = 36;
    int labelH = 12;
    int btnH = 16;
    int comboH = 18;
    int topPad = 16;

    auto layoutBand = [&] (int bandIndex) -> juce::Rectangle<int>
    {
        return juce::Rectangle<int> (bandIndex * bandW, topPad,
                                      bandW, bounds.getHeight() - topPad).reduced (4, 2);
    };

    // Band 1: LC — [On] [Freq] [Slope]
    {
        auto b = layoutBand (0);
        lcOnButton.setBounds (b.removeFromTop (btnH).withSizeKeepingCentre (32, btnH));
        b.removeFromTop (2);
        lcFreqSlider.setBounds (b.removeFromTop (knobSize).withSizeKeepingCentre (knobSize, knobSize));
        lcFreqLabel.setBounds (b.removeFromTop (labelH));
        b.removeFromTop (2);
        lcSlopeBox.setBounds (b.removeFromTop (comboH).reduced (4, 0));
    }

    // Band 2: LS — [On] [Freq] [Gain]
    {
        auto b = layoutBand (1);
        lsOnButton.setBounds (b.removeFromTop (btnH).withSizeKeepingCentre (32, btnH));
        b.removeFromTop (2);
        auto knobRow = b.removeFromTop (knobSize);
        int halfW = knobRow.getWidth() / 2;
        lsFreqSlider.setBounds (knobRow.removeFromLeft (halfW).withSizeKeepingCentre (knobSize, knobSize));
        lsGainSlider.setBounds (knobRow.withSizeKeepingCentre (knobSize, knobSize));
        auto labelRow = b.removeFromTop (labelH);
        halfW = labelRow.getWidth() / 2;
        lsFreqLabel.setBounds (labelRow.removeFromLeft (halfW));
        lsGainLabel.setBounds (labelRow);
    }

    // Band 3: Bell 1 — [Freq] [Gain] [Q]
    {
        auto b = layoutBand (2);
        b.removeFromTop (btnH + 2);
        auto knobRow = b.removeFromTop (knobSize);
        int thirdW = knobRow.getWidth() / 3;
        band1FreqSlider.setBounds (knobRow.removeFromLeft (thirdW).withSizeKeepingCentre (knobSize, knobSize));
        band1GainSlider.setBounds (knobRow.removeFromLeft (thirdW).withSizeKeepingCentre (knobSize, knobSize));
        band1QSlider.setBounds (knobRow.withSizeKeepingCentre (knobSize, knobSize));
        auto labelRow = b.removeFromTop (labelH);
        thirdW = labelRow.getWidth() / 3;
        band1FreqLabel.setBounds (labelRow.removeFromLeft (thirdW));
        band1GainLabel.setBounds (labelRow.removeFromLeft (thirdW));
        band1QLabel.setBounds (labelRow);
    }

    // Band 4: Bell 2 — [Freq] [Gain] [Q]
    {
        auto b = layoutBand (3);
        b.removeFromTop (btnH + 2);
        auto knobRow = b.removeFromTop (knobSize);
        int thirdW = knobRow.getWidth() / 3;
        band2FreqSlider.setBounds (knobRow.removeFromLeft (thirdW).withSizeKeepingCentre (knobSize, knobSize));
        band2GainSlider.setBounds (knobRow.removeFromLeft (thirdW).withSizeKeepingCentre (knobSize, knobSize));
        band2QSlider.setBounds (knobRow.withSizeKeepingCentre (knobSize, knobSize));
        auto labelRow = b.removeFromTop (labelH);
        thirdW = labelRow.getWidth() / 3;
        band2FreqLabel.setBounds (labelRow.removeFromLeft (thirdW));
        band2GainLabel.setBounds (labelRow.removeFromLeft (thirdW));
        band2QLabel.setBounds (labelRow);
    }

    // Band 5: HS — [On] [Freq] [Gain]
    {
        auto b = layoutBand (4);
        hsOnButton.setBounds (b.removeFromTop (btnH).withSizeKeepingCentre (32, btnH));
        b.removeFromTop (2);
        auto knobRow = b.removeFromTop (knobSize);
        int halfW = knobRow.getWidth() / 2;
        hsFreqSlider.setBounds (knobRow.removeFromLeft (halfW).withSizeKeepingCentre (knobSize, knobSize));
        hsGainSlider.setBounds (knobRow.withSizeKeepingCentre (knobSize, knobSize));
        auto labelRow = b.removeFromTop (labelH);
        halfW = labelRow.getWidth() / 2;
        hsFreqLabel.setBounds (labelRow.removeFromLeft (halfW));
        hsGainLabel.setBounds (labelRow);
    }

    // Band 6: HC — [On] [Freq] [Slope]
    {
        auto b = layoutBand (5);
        hcOnButton.setBounds (b.removeFromTop (btnH).withSizeKeepingCentre (32, btnH));
        b.removeFromTop (2);
        hcFreqSlider.setBounds (b.removeFromTop (knobSize).withSizeKeepingCentre (knobSize, knobSize));
        hcFreqLabel.setBounds (b.removeFromTop (labelH));
        b.removeFromTop (2);
        hcSlopeBox.setBounds (b.removeFromTop (comboH).reduced (4, 0));
    }
}
