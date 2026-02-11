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

#include "FeatureBar.h"
#include "../../PluginProcessor.h"

FeatureBar::FeatureBar (AdvancedCompressorProcessor& p)
    : processor (p)
{
    auto& apvts = processor.getAPVTS();

    // --- Compact knob helper ---
    auto setupCompactKnob = [this] (juce::Slider& slider, juce::Label& label,
                                     const juce::String& text, const juce::String& tooltip)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        slider.setTooltip (tooltip);
        addAndMakeVisible (slider);

        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (9.0f);
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    setupCompactKnob (inputGainSlider, inputGainLabel, "In", "Input Gain");
    setupCompactKnob (lookaheadSlider, lookaheadLabel, "LA", "Lookahead");
    setupCompactKnob (charDriveSlider, charDriveLabel, "Drive", "Character Drive");

    // --- Toggle button helper ---
    auto setupToggle = [this] (juce::TextButton& button)
    {
        button.setClickingTogglesState (true);
        button.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        button.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
        button.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.25f));
        button.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
        addAndMakeVisible (button);
    };

    setupToggle (autoGainButton);
    setupToggle (autoThreshButton);
    setupToggle (analyzerPreButton);
    setupToggle (midiTriggerButton);

    // --- Combo box helper ---
    auto setupCombo = [this] (juce::ComboBox& combo, juce::Label& label,
                               const juce::String& text, const juce::StringArray& items)
    {
        combo.addItemList (items, 1);
        combo.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (combo);

        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centredRight);
        label.setFont (9.0f);
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    setupCombo (charModeSelector, charModeLabel, "Char", ParamIDs::getCharacterModeNames());
    setupCombo (meterScaleSelector, meterScaleLabel, "Meter", ParamIDs::getMeterScaleNames());

    // --- Create attachments ---
    inputGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        apvts, ParamIDs::kInputGain, inputGainSlider);
    lookaheadAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        apvts, ParamIDs::kLookahead, lookaheadSlider);
    charDriveAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        apvts, ParamIDs::kCharDrive, charDriveSlider);

    autoGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kAutoGain, autoGainButton);
    autoThreshAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kAutoThresh, autoThreshButton);
    analyzerPreAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kAnalyzerPre, analyzerPreButton);
    midiTriggerAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kMIDITrigger, midiTriggerButton);

    charModeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        apvts, ParamIDs::kCharMode, charModeSelector);
    meterScaleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        apvts, ParamIDs::kMeterScale, meterScaleSelector);
}

FeatureBar::~FeatureBar()
{
    inputGainAttach.reset();
    lookaheadAttach.reset();
    charDriveAttach.reset();
    autoGainAttach.reset();
    autoThreshAttach.reset();
    analyzerPreAttach.reset();
    midiTriggerAttach.reset();
    charModeAttach.reset();
    meterScaleAttach.reset();
}

void FeatureBar::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Gradient background matching other toolbars
    juce::ColourGradient grad (ColourPalette::panelTop(), 0, bounds.getY(),
                               ColourPalette::panelBottom(), 0, bounds.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRect (bounds);

    // Top bevel
    g.setColour (ColourPalette::bevelLight());
    g.drawLine (0.0f, bounds.getY() + 0.5f, bounds.getRight(), bounds.getY() + 0.5f, 1.0f);
    g.setColour (ColourPalette::bevelDark());
    g.drawLine (0.0f, bounds.getY() + 1.5f, bounds.getRight(), bounds.getY() + 1.5f, 1.0f);
}

void FeatureBar::resized()
{
    auto bounds = getLocalBounds().reduced (8, 2);
    int h = bounds.getHeight();
    int knobSize = 28;
    int comboH = 20;
    int btnH = 22;

    // --- Left group: Input Gain knob + Lookahead knob ---
    auto inArea = bounds.removeFromLeft (36);
    inputGainSlider.setBounds (inArea.removeFromTop (h - 10).withSizeKeepingCentre (knobSize, knobSize));
    inputGainLabel.setBounds (inArea);

    auto laArea = bounds.removeFromLeft (36);
    lookaheadSlider.setBounds (laArea.removeFromTop (h - 10).withSizeKeepingCentre (knobSize, knobSize));
    lookaheadLabel.setBounds (laArea);

    bounds.removeFromLeft (6);

    // Divider position 1
    // (drawn in paint if needed, but layout keeps going)

    // --- Center-left: Auto Gain, Auto Threshold ---
    auto agArea = bounds.removeFromLeft (60);
    autoGainButton.setBounds (agArea.withHeight (btnH).withCentre (agArea.getCentre()));
    bounds.removeFromLeft (4);

    auto atArea = bounds.removeFromLeft (56);
    autoThreshButton.setBounds (atArea.withHeight (btnH).withCentre (atArea.getCentre()));
    bounds.removeFromLeft (6);

    // --- Center: Character Mode combo + Drive knob ---
    auto charLblArea = bounds.removeFromLeft (28);
    charModeLabel.setBounds (charLblArea.withHeight (comboH).withCentre (charLblArea.getCentre()));

    auto charArea = bounds.removeFromLeft (60);
    charModeSelector.setBounds (charArea.withHeight (comboH).withCentre (charArea.getCentre()));
    bounds.removeFromLeft (4);

    auto driveArea = bounds.removeFromLeft (36);
    charDriveSlider.setBounds (driveArea.removeFromTop (h - 10).withSizeKeepingCentre (knobSize, knobSize));
    charDriveLabel.setBounds (driveArea);

    bounds.removeFromLeft (6);

    // --- Right group (anchored to right edge) ---
    auto rightBounds = bounds;

    auto midiArea = rightBounds.removeFromRight (58);
    midiTriggerButton.setBounds (midiArea.withHeight (btnH).withCentre (midiArea.getCentre()));
    rightBounds.removeFromRight (4);

    auto meterLblArea = rightBounds.removeFromRight (32);
    meterScaleLabel.setBounds (meterLblArea.withHeight (comboH).withCentre (meterLblArea.getCentre()));

    auto meterArea = rightBounds.removeFromRight (52);
    meterScaleSelector.setBounds (meterArea.withHeight (comboH).withCentre (meterArea.getCentre()));
    rightBounds.removeFromRight (4);

    auto preArea = rightBounds.removeFromRight (30);
    analyzerPreButton.setBounds (preArea.withHeight (btnH).withCentre (preArea.getCentre()));
}
