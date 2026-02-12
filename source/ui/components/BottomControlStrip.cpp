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

#include "BottomControlStrip.h"
#include "../../PluginProcessor.h"
#include "../LookAndFeel/ColourPalette.h"
#include "../LookAndFeel/Fonts.h"

BottomControlStrip::BottomControlStrip (AdvancedCompressorProcessor& p)
    : processor (p),
      sidechainPanel (p.getAPVTS())
{
    auto& apvts = processor.getAPVTS();

    // ========================= Tab bar setup =========================

    auto setupTabButton = [this] (juce::TextButton& btn, int index)
    {
        btn.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        btn.setColour (juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
        btn.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
        btn.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
        btn.onClick = [this, index] { setActiveTab (index); };
        addAndMakeVisible (btn);
    };

    setupTabButton (basicTab, 0);
    setupTabButton (advancedTab, 1);
    setupTabButton (sidechainTab, 2);

    addAndMakeVisible (basicPanel);
    addAndMakeVisible (advancedPanel);
    addAndMakeVisible (sidechainContent);

    // ========================= Basic tab setup =========================

    auto setupLargeKnob = [] (juce::Component& parent, juce::Slider& slider, juce::Label& title,
                               juce::Label& value, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        parent.addAndMakeVisible (slider);

        title.setText (text, juce::dontSendNotification);
        title.setJustificationType (juce::Justification::centred);
        title.setFont (Fonts::getBody (10.0f));
        title.setColour (juce::Label::textColourId, ColourPalette::textDim());
        parent.addAndMakeVisible (title);

        value.setJustificationType (juce::Justification::centred);
        value.setFont (Fonts::getBody (11.0f));
        value.setColour (juce::Label::textColourId, ColourPalette::axisText());
        parent.addAndMakeVisible (value);
    };

    setupLargeKnob (basicPanel, thresholdSlider, thresholdTitle, thresholdValue, "Threshold");
    setupLargeKnob (basicPanel, ratioSlider, ratioTitle, ratioValue, "Ratio");
    setupLargeKnob (basicPanel, attackSlider, attackTitle, attackValue, "Attack");
    setupLargeKnob (basicPanel, releaseSlider, releaseTitle, releaseValue, "Release");
    setupLargeKnob (basicPanel, mixSlider, mixTitle, mixValue, "Mix");
    setupLargeKnob (basicPanel, outputGainSlider, outputGainTitle, outputGainValue, "Output");

    thresholdSlider.setComponentID ("bottom.threshold");
    ratioSlider.setComponentID ("bottom.ratio");
    attackSlider.setComponentID ("bottom.attack");
    releaseSlider.setComponentID ("bottom.release");
    mixSlider.setComponentID ("bottom.mix");
    outputGainSlider.setComponentID ("bottom.outputGain");

    thresholdSlider.onValueChange = [this] { updateValueLabels(); };
    ratioSlider.onValueChange = [this] { updateValueLabels(); };
    attackSlider.onValueChange = [this] { updateValueLabels(); };
    releaseSlider.onValueChange = [this] { updateValueLabels(); };
    mixSlider.onValueChange = [this] { updateValueLabels(); };
    outputGainSlider.onValueChange = [this] { updateValueLabels(); };

    basicPanel.addAndMakeVisible (thresholdRing);

    // Style selector (centered between knob groups)
    styleSelector.addItemList (ParamIDs::getStyleNames(), 1);
    styleSelector.setJustificationType (juce::Justification::centred);
    styleSelector.setComponentID ("bottom.style");
    basicPanel.addAndMakeVisible (styleSelector);
    styleLabel.setText ("Style", juce::dontSendNotification);
    styleLabel.setJustificationType (juce::Justification::centred);
    styleLabel.setFont (Fonts::getBody (10.0f));
    styleLabel.setColour (juce::Label::textColourId, ColourPalette::textDim());
    basicPanel.addAndMakeVisible (styleLabel);

    // Analyzer toggle
    analyzerButton.setClickingTogglesState (true);
    analyzerButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    analyzerButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    analyzerButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::spectrumPost().withAlpha (0.3f));
    analyzerButton.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
    analyzerButton.setComponentID ("bottom.analyzer");
    basicPanel.addAndMakeVisible (analyzerButton);

    // Auto Gain toggle
    autoGainButton.setClickingTogglesState (true);
    autoGainButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    autoGainButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    autoGainButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.25f));
    autoGainButton.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
    autoGainButton.setComponentID ("bottom.autoGain");
    basicPanel.addAndMakeVisible (autoGainButton);

    // ========================= Advanced tab setup =========================

    auto setupCompactKnob = [] (juce::Component& parent, juce::Slider& slider, juce::Label& title,
                                 juce::Label& value, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        parent.addAndMakeVisible (slider);

        title.setText (text, juce::dontSendNotification);
        title.setJustificationType (juce::Justification::centred);
        title.setFont (Fonts::getBody (10.0f));
        title.setColour (juce::Label::textColourId, ColourPalette::textDim());
        parent.addAndMakeVisible (title);

        value.setJustificationType (juce::Justification::centred);
        value.setFont (Fonts::getBody (11.0f));
        value.setColour (juce::Label::textColourId, ColourPalette::axisText());
        parent.addAndMakeVisible (value);
    };

    // Group A: Dynamics refinement
    setupCompactKnob (advancedPanel, kneeSlider, kneeTitle, kneeValue, "Knee");
    setupCompactKnob (advancedPanel, rangeSlider, rangeTitle, rangeValue, "Range");
    setupCompactKnob (advancedPanel, holdSlider, holdTitle, holdValue, "Hold");
    setupCompactKnob (advancedPanel, lookaheadSlider, lookaheadTitle, lookaheadValue, "LA");

    kneeSlider.setComponentID ("bottom.knee");
    rangeSlider.setComponentID ("bottom.range");
    holdSlider.setComponentID ("bottom.hold");
    lookaheadSlider.setComponentID ("bottom.lookahead");

    kneeSlider.onValueChange = [this] { updateValueLabels(); };
    rangeSlider.onValueChange = [this] { updateValueLabels(); };
    holdSlider.onValueChange = [this] { updateValueLabels(); };
    lookaheadSlider.onValueChange = [this] { updateValueLabels(); };

    // Group B: Output/stereo
    setupCompactKnob (advancedPanel, wetGainSlider, wetGainTitle, wetGainValue, "Wet");
    setupCompactKnob (advancedPanel, stereoLinkSlider, stereoLinkTitle, stereoLinkValue, "Link");

    wetGainSlider.setComponentID ("bottom.wetGain");
    stereoLinkSlider.setComponentID ("bottom.stereoLink");

    wetGainSlider.onValueChange = [this] { updateValueLabels(); };
    stereoLinkSlider.onValueChange = [this] { updateValueLabels(); };

    stereoModeSelector.addItemList (ParamIDs::getStereoModeNames(), 1);
    stereoModeSelector.setJustificationType (juce::Justification::centred);
    stereoModeSelector.setComponentID ("bottom.stereoMode");
    stereoModeLabel.setText ("Mode", juce::dontSendNotification);
    stereoModeLabel.setJustificationType (juce::Justification::centred);
    stereoModeLabel.setFont (Fonts::getBody (10.0f));
    stereoModeLabel.setColour (juce::Label::textColourId, ColourPalette::textDim());
    advancedPanel.addAndMakeVisible (stereoModeSelector);
    advancedPanel.addAndMakeVisible (stereoModeLabel);

    // Group C: Input/character
    setupCompactKnob (advancedPanel, inputGainSlider, inputGainTitle, inputGainValue, "In");
    setupCompactKnob (advancedPanel, charDriveSlider, charDriveTitle, charDriveValue, "Drive");

    inputGainSlider.setComponentID ("bottom.inputGain");
    charDriveSlider.setComponentID ("bottom.charDrive");

    inputGainSlider.onValueChange = [this] { updateValueLabels(); };
    charDriveSlider.onValueChange = [this] { updateValueLabels(); };

    charModeSelector.addItemList (ParamIDs::getCharacterModeNames(), 1);
    charModeSelector.setJustificationType (juce::Justification::centred);
    charModeSelector.setComponentID ("bottom.charMode");
    charModeLabel.setText ("Char", juce::dontSendNotification);
    charModeLabel.setJustificationType (juce::Justification::centredRight);
    charModeLabel.setFont (Fonts::getBody (10.0f));
    charModeLabel.setColour (juce::Label::textColourId, ColourPalette::textDim());
    advancedPanel.addAndMakeVisible (charModeSelector);
    advancedPanel.addAndMakeVisible (charModeLabel);

    // ========================= Utility strip (always visible) =========================

    auto setupUtilToggle = [this] (juce::TextButton& button)
    {
        button.setClickingTogglesState (true);
        button.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        button.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
        button.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.25f));
        button.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
        addAndMakeVisible (button);
    };

    auto setupUtilCombo = [this] (juce::ComboBox& combo, juce::Label& label,
                                   const juce::String& text, const juce::StringArray& items)
    {
        combo.addItemList (items, 1);
        combo.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (combo);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centredRight);
        label.setFont (Fonts::getBody (11.0f));
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    setupUtilCombo (oversampleSelector, oversampleLabel, "OS", ParamIDs::getOversamplingNames());
    setupUtilCombo (meterScaleSelector, meterScaleLabel, "Meter", ParamIDs::getMeterScaleNames());
    oversampleSelector.setComponentID ("bottom.oversample");
    meterScaleSelector.setComponentID ("bottom.meterScale");

    setupUtilToggle (autoThreshButton);
    setupUtilToggle (midiTriggerButton);
    setupUtilToggle (analyzerPreButton);

    autoThreshButton.setComponentID ("bottom.autoThresh");
    midiTriggerButton.setComponentID ("bottom.midiTrigger");
    analyzerPreButton.setComponentID ("bottom.analyzerPre");

    // ========================= Sidechain tab setup =========================
    sidechainContent.addAndMakeVisible (sidechainPanel);

    // ========================= Attachments =========================
    // Basic tab
    thresholdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kThreshold, thresholdSlider);
    ratioAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRatio, ratioSlider);
    attackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kAttack, attackSlider);
    releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRelease, releaseSlider);
    mixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kMix, mixSlider);
    outputGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kOutputGain, outputGainSlider);
    styleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kCompStyle, styleSelector);
    analyzerAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kAnalyzerOn, analyzerButton);
    autoGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kAutoGain, autoGainButton);

    // Advanced tab
    kneeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kKnee, kneeSlider);
    rangeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRange, rangeSlider);
    holdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kHold, holdSlider);
    lookaheadAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kLookahead, lookaheadSlider);
    wetGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kWetGain, wetGainSlider);
    stereoLinkAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kStereoLink, stereoLinkSlider);
    stereoModeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kStereoMode, stereoModeSelector);
    inputGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kInputGain, inputGainSlider);
    charDriveAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kCharDrive, charDriveSlider);
    charModeAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kCharMode, charModeSelector);
    oversampleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kOversample, oversampleSelector);
    meterScaleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kMeterScale, meterScaleSelector);
    autoThreshAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kAutoThresh, autoThreshButton);
    midiTriggerAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kMIDITrigger, midiTriggerButton);
    analyzerPreAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kAnalyzerPre, analyzerPreButton);

    // Initial value label update
    updateValueLabels();

    // Set initial tab
    setActiveTab (0);
}

BottomControlStrip::~BottomControlStrip()
{
    // Basic tab
    thresholdAttach.reset(); ratioAttach.reset();
    attackAttach.reset(); releaseAttach.reset();
    mixAttach.reset(); outputGainAttach.reset();
    styleAttach.reset(); analyzerAttach.reset(); autoGainAttach.reset();
    // Advanced tab
    kneeAttach.reset(); rangeAttach.reset(); holdAttach.reset(); lookaheadAttach.reset();
    wetGainAttach.reset(); stereoLinkAttach.reset(); stereoModeAttach.reset();
    inputGainAttach.reset(); charDriveAttach.reset(); charModeAttach.reset();
    oversampleAttach.reset(); meterScaleAttach.reset();
    autoThreshAttach.reset(); midiTriggerAttach.reset(); analyzerPreAttach.reset();
}

void BottomControlStrip::setGainReduction (float grDB)
{
    thresholdRing.setGainReduction (grDB);
}

void BottomControlStrip::setActiveTab (int tab)
{
    activeTab = tab;
    basicPanel.setVisible (tab == 0);
    advancedPanel.setVisible (tab == 1);
    sidechainContent.setVisible (tab == 2);
    basicTab.setToggleState (tab == 0, juce::dontSendNotification);
    advancedTab.setToggleState (tab == 1, juce::dontSendNotification);
    sidechainTab.setToggleState (tab == 2, juce::dontSendNotification);
    repaint();
}

void BottomControlStrip::updateValueLabels()
{
    // Basic tab
    thresholdValue.setText (juce::String (thresholdSlider.getValue(), 1) + " dB", juce::dontSendNotification);

    auto ratio = ratioSlider.getValue();
    if (ratio >= 100.0)
        ratioValue.setText ("Inf:1", juce::dontSendNotification);
    else
        ratioValue.setText (juce::String (ratio, 1) + ":1", juce::dontSendNotification);

    attackValue.setText (juce::String (attackSlider.getValue(), 1) + " ms", juce::dontSendNotification);
    releaseValue.setText (juce::String (releaseSlider.getValue(), 0) + " ms", juce::dontSendNotification);
    mixValue.setText (juce::String ((int) mixSlider.getValue()) + "%", juce::dontSendNotification);
    outputGainValue.setText (juce::String (outputGainSlider.getValue(), 1) + " dB", juce::dontSendNotification);

    // Advanced tab
    kneeValue.setText (juce::String (kneeSlider.getValue(), 1) + " dB", juce::dontSendNotification);
    rangeValue.setText (juce::String (rangeSlider.getValue(), 1) + " dB", juce::dontSendNotification);
    holdValue.setText (juce::String (holdSlider.getValue(), 1) + " ms", juce::dontSendNotification);
    lookaheadValue.setText (juce::String (lookaheadSlider.getValue(), 1) + " ms", juce::dontSendNotification);
    wetGainValue.setText (juce::String (wetGainSlider.getValue(), 1) + " dB", juce::dontSendNotification);
    stereoLinkValue.setText (juce::String ((int) stereoLinkSlider.getValue()) + "%", juce::dontSendNotification);
    inputGainValue.setText (juce::String (inputGainSlider.getValue(), 1) + " dB", juce::dontSendNotification);
    charDriveValue.setText (juce::String (charDriveSlider.getValue(), 1) + " dB", juce::dontSendNotification);
}

void BottomControlStrip::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Panel gradient over full bounds
    juce::ColourGradient grad (ColourPalette::panelTop(), 0, bounds.getY(),
                               ColourPalette::panelBottom(), 0, bounds.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRect (bounds);

    // Top bevel
    g.setColour (ColourPalette::bevelLight());
    g.drawLine (0.0f, bounds.getY() + 0.5f, bounds.getRight(), bounds.getY() + 0.5f, 1.0f);
    g.setColour (ColourPalette::bevelDark());
    g.drawLine (0.0f, bounds.getY() + 1.5f, bounds.getRight(), bounds.getY() + 1.5f, 1.0f);

    // Draw 2px accent underline under active tab
    juce::TextButton* tabs[] = { &basicTab, &advancedTab, &sidechainTab };
    auto tabBounds = tabs[activeTab]->getBounds().toFloat();
    g.setColour (ColourPalette::thresholdAccent());
    g.fillRect (tabBounds.getX(), tabBounds.getBottom() - 2.0f, tabBounds.getWidth(), 2.0f);

    // Separator line above utility strip (always visible)
    float utilStripH = 30.0f;
    float sepY = bounds.getBottom() - utilStripH;
    g.setColour (ColourPalette::gridLine().withAlpha (0.5f));
    g.drawLine (8.0f, sepY, bounds.getRight() - 8.0f, sepY, 0.5f);
}

void BottomControlStrip::resized()
{
    auto bounds = getLocalBounds();

    // Tab bar at the top
    auto tabBar = bounds.removeFromTop (28);
    int tabW = 80;
    int totalTabsW = tabW * 3;
    auto tabArea = tabBar.withSizeKeepingCentre (totalTabsW, tabBar.getHeight());
    basicTab.setBounds (tabArea.removeFromLeft (tabW));
    advancedTab.setBounds (tabArea.removeFromLeft (tabW));
    sidechainTab.setBounds (tabArea.removeFromLeft (tabW));

    // Utility strip at the bottom (always visible)
    auto utilityStrip = bounds.removeFromBottom (30);

    // Content area (same bounds for all three panels)
    auto contentArea = bounds;
    basicPanel.setBounds (contentArea);
    advancedPanel.setBounds (contentArea);
    sidechainContent.setBounds (contentArea);

    // ==================== Basic tab layout ====================
    {
        auto area = basicPanel.getLocalBounds();
        int largeKnobW = 80;
        int styleColW = 100;
        int basicContentW = largeKnobW * 6 + styleColW + 4 + 8;
        auto row = area.withSizeKeepingCentre (basicContentW, area.getHeight()).reduced (0, 4);

        auto layoutLargeKnob = [] (juce::Slider& slider, juce::Label& title,
                                    juce::Label& value, juce::Rectangle<int>& r, int knobW)
        {
            auto col = r.removeFromLeft (knobW);
            title.setBounds (col.removeFromTop (14));
            value.setBounds (col.removeFromBottom (14));
            slider.setBounds (col);
        };

        // Threshold with ring overlay
        {
            auto col = row.removeFromLeft (largeKnobW);
            thresholdTitle.setBounds (col.removeFromTop (14));
            thresholdValue.setBounds (col.removeFromBottom (14));
            thresholdSlider.setBounds (col);
            thresholdRing.setBounds (col);
        }

        layoutLargeKnob (ratioSlider, ratioTitle, ratioValue, row, largeKnobW);

        // Style selector (centered column)
        {
            auto styleCol = row.removeFromLeft (styleColW);
            int comboH = 22;
            int btnH = 20;
            auto top = styleCol.removeFromTop (comboH + 4);
            styleSelector.setBounds (top.withHeight (comboH).withSizeKeepingCentre (90, comboH));
            styleLabel.setBounds (styleCol.removeFromTop (14));

            auto btnArea = styleCol;
            analyzerButton.setBounds (btnArea.removeFromTop (btnH).reduced (4, 0));
            autoGainButton.setBounds (btnArea.removeFromTop (btnH).reduced (4, 0));
        }

        row.removeFromLeft (4);

        layoutLargeKnob (attackSlider, attackTitle, attackValue, row, largeKnobW);
        layoutLargeKnob (releaseSlider, releaseTitle, releaseValue, row, largeKnobW);

        row.removeFromLeft (8);

        layoutLargeKnob (mixSlider, mixTitle, mixValue, row, largeKnobW);
        layoutLargeKnob (outputGainSlider, outputGainTitle, outputGainValue, row, largeKnobW);
    }

    // ==================== Advanced tab layout ====================
    {
        auto area = advancedPanel.getLocalBounds();
        auto knobArea = area.reduced (0, 3);
        int compactKnobW = 52;
        int modeW = 48;
        int advKnobContentW = compactKnobW * 8 + modeW * 2 + 6 * 2;
        auto knobRow = knobArea.withSizeKeepingCentre (advKnobContentW, knobArea.getHeight());

        auto layoutCompactKnob = [] (juce::Slider& slider, juce::Label& title,
                                      juce::Label& value, juce::Rectangle<int>& r, int knobW)
        {
            auto col = r.removeFromLeft (knobW);
            title.setBounds (col.removeFromTop (14));
            value.setBounds (col.removeFromBottom (14));
            slider.setBounds (col);
        };

        // Group A: Knee, Range, Hold, Lookahead
        layoutCompactKnob (kneeSlider, kneeTitle, kneeValue, knobRow, compactKnobW);
        layoutCompactKnob (rangeSlider, rangeTitle, rangeValue, knobRow, compactKnobW);
        layoutCompactKnob (holdSlider, holdTitle, holdValue, knobRow, compactKnobW);
        layoutCompactKnob (lookaheadSlider, lookaheadTitle, lookaheadValue, knobRow, compactKnobW);

        knobRow.removeFromLeft (6);

        // Group B: Wet, Link, Mode
        layoutCompactKnob (wetGainSlider, wetGainTitle, wetGainValue, knobRow, compactKnobW);
        layoutCompactKnob (stereoLinkSlider, stereoLinkTitle, stereoLinkValue, knobRow, compactKnobW);

        {
            auto modeCol = knobRow.removeFromLeft (modeW);
            auto modeKnobH = modeCol.getHeight() - 28;
            stereoModeSelector.setBounds (modeCol.removeFromTop (14 + modeKnobH).reduced (0, modeKnobH / 4 + 7));
            stereoModeLabel.setBounds (modeCol);
        }

        knobRow.removeFromLeft (6);

        // Group C: Input, Drive, Char
        layoutCompactKnob (inputGainSlider, inputGainTitle, inputGainValue, knobRow, compactKnobW);
        layoutCompactKnob (charDriveSlider, charDriveTitle, charDriveValue, knobRow, compactKnobW);

        {
            auto charCol = knobRow.removeFromLeft (modeW);
            auto charKnobH = charCol.getHeight() - 28;
            charModeSelector.setBounds (charCol.removeFromTop (14 + charKnobH).reduced (0, charKnobH / 4 + 7));
            charModeLabel.setBounds (charCol);
        }
    }

    // ==================== Sidechain tab layout ====================
    sidechainPanel.setBounds (sidechainContent.getLocalBounds());

    // ==================== Utility strip layout (always visible, centered) ====================
    {
        int comboH = 22;
        int btnH = 22;
        // OS(24+58) + Auto Thr(68) + Meter(40+62) + MIDI Trg(68) + Pre(38) + gaps(14*4)
        int utilityContentW = 24 + 58 + 14 + 68 + 14 + 40 + 62 + 14 + 68 + 14 + 38;
        auto utilRow = utilityStrip.withSizeKeepingCentre (utilityContentW, utilityStrip.getHeight());

        // Oversample
        auto osLbl = utilRow.removeFromLeft (24);
        oversampleLabel.setBounds (osLbl.withHeight (comboH).withCentre (osLbl.getCentre()));
        auto osArea = utilRow.removeFromLeft (58);
        oversampleSelector.setBounds (osArea.withHeight (comboH).withCentre (osArea.getCentre()));
        utilRow.removeFromLeft (14);

        // Auto Threshold
        auto atArea = utilRow.removeFromLeft (68);
        autoThreshButton.setBounds (atArea.withHeight (btnH).withCentre (atArea.getCentre()));
        utilRow.removeFromLeft (14);

        // Meter Scale
        auto mlbl = utilRow.removeFromLeft (40);
        meterScaleLabel.setBounds (mlbl.withHeight (comboH).withCentre (mlbl.getCentre()));
        auto mArea = utilRow.removeFromLeft (62);
        meterScaleSelector.setBounds (mArea.withHeight (comboH).withCentre (mArea.getCentre()));
        utilRow.removeFromLeft (14);

        // MIDI Trigger
        auto mtArea = utilRow.removeFromLeft (68);
        midiTriggerButton.setBounds (mtArea.withHeight (btnH).withCentre (mtArea.getCentre()));
        utilRow.removeFromLeft (14);

        // Analyzer Pre
        auto preArea = utilRow.removeFromLeft (38);
        analyzerPreButton.setBounds (preArea.withHeight (btnH).withCentre (preArea.getCentre()));
    }
}
