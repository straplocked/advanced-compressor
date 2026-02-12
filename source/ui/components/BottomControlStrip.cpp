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
    : processor (p)
{
    auto& apvts = processor.getAPVTS();

    // ========================= Row 1 setup =========================

    auto setupLargeKnob = [this] (juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 14);
        addAndMakeVisible (slider);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (Fonts::getBody (11.0f));
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    setupLargeKnob (thresholdSlider, thresholdLabel, "Threshold");
    setupLargeKnob (ratioSlider, ratioLabel, "Ratio");
    setupLargeKnob (attackSlider, attackLabel, "Attack");
    setupLargeKnob (releaseSlider, releaseLabel, "Release");
    setupLargeKnob (outputGainSlider, outputGainLabel, "Output");

    thresholdSlider.setComponentID ("bottom.threshold");
    ratioSlider.setComponentID ("bottom.ratio");
    attackSlider.setComponentID ("bottom.attack");
    releaseSlider.setComponentID ("bottom.release");
    outputGainSlider.setComponentID ("bottom.outputGain");

    addAndMakeVisible (thresholdRing);

    // Style selector (centered between knob groups)
    styleSelector.addItemList (ParamIDs::getStyleNames(), 1);
    styleSelector.setJustificationType (juce::Justification::centred);
    styleSelector.setComponentID ("bottom.style");
    addAndMakeVisible (styleSelector);
    styleLabel.setText ("Style", juce::dontSendNotification);
    styleLabel.setJustificationType (juce::Justification::centred);
    styleLabel.setFont (Fonts::getBody (10.0f));
    styleLabel.setColour (juce::Label::textColourId, ColourPalette::axisText());
    addAndMakeVisible (styleLabel);

    // Analyzer toggle
    analyzerButton.setClickingTogglesState (true);
    analyzerButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    analyzerButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    analyzerButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::spectrumPost().withAlpha (0.3f));
    analyzerButton.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
    analyzerButton.setComponentID ("bottom.analyzer");
    addAndMakeVisible (analyzerButton);

    // Auto Gain toggle
    autoGainButton.setClickingTogglesState (true);
    autoGainButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    autoGainButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    autoGainButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.25f));
    autoGainButton.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
    autoGainButton.setComponentID ("bottom.autoGain");
    addAndMakeVisible (autoGainButton);

    // ========================= Row 2 setup =========================

    auto setupCompactKnob = [this] (juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        addAndMakeVisible (slider);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (Fonts::getBody (10.0f));
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    // Group A: Dynamics refinement
    setupCompactKnob (kneeSlider, kneeLabel, "Knee");
    setupCompactKnob (rangeSlider, rangeLabel, "Range");
    setupCompactKnob (holdSlider, holdLabel, "Hold");
    setupCompactKnob (lookaheadSlider, lookaheadLabel, "LA");

    kneeSlider.setComponentID ("bottom.knee");
    rangeSlider.setComponentID ("bottom.range");
    holdSlider.setComponentID ("bottom.hold");
    lookaheadSlider.setComponentID ("bottom.lookahead");

    // Group B: Output/stereo
    setupCompactKnob (mixSlider, mixLabel, "Mix");
    setupCompactKnob (wetGainSlider, wetGainLabel, "Wet");
    setupCompactKnob (stereoLinkSlider, stereoLinkLabel, "Link");

    mixSlider.setComponentID ("bottom.mix");
    wetGainSlider.setComponentID ("bottom.wetGain");
    stereoLinkSlider.setComponentID ("bottom.stereoLink");

    stereoModeSelector.addItemList (ParamIDs::getStereoModeNames(), 1);
    stereoModeSelector.setJustificationType (juce::Justification::centred);
    stereoModeSelector.setComponentID ("bottom.stereoMode");
    stereoModeLabel.setText ("Mode", juce::dontSendNotification);
    stereoModeLabel.setJustificationType (juce::Justification::centred);
    stereoModeLabel.setFont (Fonts::getBody (10.0f));
    stereoModeLabel.setColour (juce::Label::textColourId, ColourPalette::axisText());
    addAndMakeVisible (stereoModeSelector);
    addAndMakeVisible (stereoModeLabel);

    // Group C: Input/character
    setupCompactKnob (inputGainSlider, inputGainLabel, "In");
    setupCompactKnob (charDriveSlider, charDriveLabel, "Drive");

    inputGainSlider.setComponentID ("bottom.inputGain");
    charDriveSlider.setComponentID ("bottom.charDrive");

    charModeSelector.addItemList (ParamIDs::getCharacterModeNames(), 1);
    charModeSelector.setJustificationType (juce::Justification::centred);
    charModeSelector.setComponentID ("bottom.charMode");
    charModeLabel.setText ("Char", juce::dontSendNotification);
    charModeLabel.setJustificationType (juce::Justification::centredRight);
    charModeLabel.setFont (Fonts::getBody (10.0f));
    charModeLabel.setColour (juce::Label::textColourId, ColourPalette::axisText());
    addAndMakeVisible (charModeSelector);
    addAndMakeVisible (charModeLabel);

    // Small buttons/combos in Row 2
    auto setupToggle = [this] (juce::TextButton& button)
    {
        button.setClickingTogglesState (true);
        button.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
        button.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
        button.setColour (juce::TextButton::buttonOnColourId, ColourPalette::thresholdAccent().withAlpha (0.25f));
        button.setColour (juce::TextButton::textColourOnId, ColourPalette::textBright());
        addAndMakeVisible (button);
    };

    auto setupCombo = [this] (juce::ComboBox& combo, juce::Label& label,
                               const juce::String& text, const juce::StringArray& items)
    {
        combo.addItemList (items, 1);
        combo.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (combo);
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centredRight);
        label.setFont (Fonts::getBody (10.0f));
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        addAndMakeVisible (label);
    };

    setupCombo (oversampleSelector, oversampleLabel, "OS", ParamIDs::getOversamplingNames());
    setupCombo (meterScaleSelector, meterScaleLabel, "Meter", ParamIDs::getMeterScaleNames());
    oversampleSelector.setComponentID ("bottom.oversample");
    meterScaleSelector.setComponentID ("bottom.meterScale");

    setupToggle (autoThreshButton);
    setupToggle (midiTriggerButton);
    setupToggle (analyzerPreButton);

    autoThreshButton.setComponentID ("bottom.autoThresh");
    midiTriggerButton.setComponentID ("bottom.midiTrigger");
    analyzerPreButton.setComponentID ("bottom.analyzerPre");

    // ========================= SC toggle bar =========================
    scToggleButton.setComponentID ("sc.toggle");
    scToggleButton.setColour (juce::TextButton::buttonColourId, ColourPalette::panelBottom().brighter (0.05f));
    scToggleButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    scToggleButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::panelBottom().brighter (0.08f));
    scToggleButton.setColour (juce::TextButton::textColourOnId, ColourPalette::thresholdAccent());
    scToggleButton.setClickingTogglesState (true);
    scToggleButton.onClick = [this]
    {
        if (onSidechainToggle)
            onSidechainToggle();
    };
    addAndMakeVisible (scToggleButton);

    // ========================= Attachments =========================
    // Row 1
    thresholdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kThreshold, thresholdSlider);
    ratioAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRatio, ratioSlider);
    attackAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kAttack, attackSlider);
    releaseAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRelease, releaseSlider);
    outputGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kOutputGain, outputGainSlider);
    styleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (apvts, ParamIDs::kCompStyle, styleSelector);
    analyzerAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kAnalyzerOn, analyzerButton);
    autoGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (apvts, ParamIDs::kAutoGain, autoGainButton);

    // Row 2
    kneeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kKnee, kneeSlider);
    rangeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kRange, rangeSlider);
    holdAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kHold, holdSlider);
    lookaheadAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kLookahead, lookaheadSlider);
    mixAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts, ParamIDs::kMix, mixSlider);
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
}

BottomControlStrip::~BottomControlStrip()
{
    // Row 1
    thresholdAttach.reset(); ratioAttach.reset();
    attackAttach.reset(); releaseAttach.reset(); outputGainAttach.reset();
    styleAttach.reset(); analyzerAttach.reset(); autoGainAttach.reset();
    // Row 2
    kneeAttach.reset(); rangeAttach.reset(); holdAttach.reset(); lookaheadAttach.reset();
    mixAttach.reset(); wetGainAttach.reset(); stereoLinkAttach.reset(); stereoModeAttach.reset();
    inputGainAttach.reset(); charDriveAttach.reset(); charModeAttach.reset();
    oversampleAttach.reset(); meterScaleAttach.reset();
    autoThreshAttach.reset(); midiTriggerAttach.reset(); analyzerPreAttach.reset();
}

void BottomControlStrip::setGainReduction (float grDB)
{
    thresholdRing.setGainReduction (grDB);
}

void BottomControlStrip::setSidechainOpen (bool open)
{
    scDrawerOpen = open;
    scToggleButton.setToggleState (open, juce::dontSendNotification);
    repaint();
}

void BottomControlStrip::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Main panel gradient (above SC bar)
    auto panelBounds = bounds.withTrimmedBottom (24.0f);
    juce::ColourGradient grad (ColourPalette::panelTop(), 0, panelBounds.getY(),
                               ColourPalette::panelBottom(), 0, panelBounds.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRect (panelBounds);

    // Top bevel
    g.setColour (ColourPalette::bevelLight());
    g.drawLine (0.0f, panelBounds.getY() + 0.5f, bounds.getRight(), panelBounds.getY() + 0.5f, 1.0f);
    g.setColour (ColourPalette::bevelDark());
    g.drawLine (0.0f, panelBounds.getY() + 1.5f, bounds.getRight(), panelBounds.getY() + 1.5f, 1.0f);

    // Row separator line
    int row1Height = (int) (panelBounds.getHeight() * 0.56f);
    float separatorY = panelBounds.getY() + row1Height;
    g.setColour (ColourPalette::gridLine().withAlpha (0.5f));
    g.drawLine (8.0f, separatorY, bounds.getRight() - 8.0f, separatorY, 0.5f);
}

void BottomControlStrip::resized()
{
    auto bounds = getLocalBounds();

    // SC toggle bar at the very bottom
    auto scBar = bounds.removeFromBottom (24);
    scToggleButton.setBounds (scBar);

    // Split remaining into Row 1 and Row 2
    int row1Height = (int) (bounds.getHeight() * 0.56f);
    auto row1Full = bounds.removeFromTop (row1Height);
    auto row2Full = bounds;

    // ==================== Row 1 Layout (centered) ====================
    int largeKnobW = 80;
    int styleColW = 100;
    int row1ContentW = largeKnobW * 5 + styleColW + 4 + 8; // 5 knobs + style + gaps
    auto row1 = row1Full.withSizeKeepingCentre (row1ContentW, row1Full.getHeight()).reduced (0, 4);

    int largeKnobH = row1.getHeight() - 16;

    auto layoutLargeKnob = [&] (juce::Slider& slider, juce::Label& label, juce::Rectangle<int>& area)
    {
        auto col = area.removeFromLeft (largeKnobW);
        auto knobArea = col.removeFromTop (largeKnobH);
        slider.setBounds (knobArea);
        label.setBounds (col);
    };

    // Threshold with ring overlay
    {
        auto col = row1.removeFromLeft (largeKnobW);
        auto knobArea = col.removeFromTop (largeKnobH);
        thresholdSlider.setBounds (knobArea);
        // Ring covers only the rotary part (above the 14px text box)
        thresholdRing.setBounds (knobArea.withTrimmedBottom (14));
        thresholdLabel.setBounds (col);
    }

    layoutLargeKnob (ratioSlider, ratioLabel, row1);

    // Style selector (centered column)
    {
        auto styleCol = row1.removeFromLeft (styleColW);
        int comboH = 22;
        int btnH = 20;
        auto top = styleCol.removeFromTop (comboH + 4);
        styleSelector.setBounds (top.withHeight (comboH).withSizeKeepingCentre (90, comboH));
        styleLabel.setBounds (styleCol.removeFromTop (14));

        auto btnArea = styleCol;
        analyzerButton.setBounds (btnArea.removeFromTop (btnH).reduced (4, 0));
        autoGainButton.setBounds (btnArea.removeFromTop (btnH).reduced (4, 0));
    }

    row1.removeFromLeft (4);

    layoutLargeKnob (attackSlider, attackLabel, row1);
    layoutLargeKnob (releaseSlider, releaseLabel, row1);

    row1.removeFromLeft (8);

    layoutLargeKnob (outputGainSlider, outputGainLabel, row1);

    // ==================== Row 2 Layout (centered) ====================
    int compactKnobW = 44;
    int modeW = 48;
    int charColW = 52;
    // Group A: 4 knobs (176) + gap(6) + Group B: 3 knobs + mode (180+48) + gap(6)
    // + Group C: 2 knobs + char (88+52) + gap(6)
    // + OS lbl+combo (62) + gap(4) + AutoThr (52) + gap(4) + Meter lbl+combo (72) + gap(4) + MIDITrg (52) + gap(4) + Pre (28)
    int row2ContentW = compactKnobW * 9 + modeW + charColW + 62 + 52 + 72 + 52 + 28 + 6 * 3 + 4 * 4;
    auto row2 = row2Full.withSizeKeepingCentre (row2ContentW, row2Full.getHeight()).reduced (0, 3);

    int compactKnobH = row2.getHeight() - 12;
    int comboH = 18;
    int btnH = 18;

    auto layoutCompactKnob = [&] (juce::Slider& slider, juce::Label& label, juce::Rectangle<int>& area)
    {
        auto col = area.removeFromLeft (compactKnobW);
        auto knobArea = col.removeFromTop (compactKnobH);
        slider.setBounds (knobArea);
        label.setBounds (col);
    };

    // Group A: Knee, Range, Hold, Lookahead
    layoutCompactKnob (kneeSlider, kneeLabel, row2);
    layoutCompactKnob (rangeSlider, rangeLabel, row2);
    layoutCompactKnob (holdSlider, holdLabel, row2);
    layoutCompactKnob (lookaheadSlider, lookaheadLabel, row2);

    row2.removeFromLeft (6);

    // Group B: Mix, Wet, Link, Mode
    layoutCompactKnob (mixSlider, mixLabel, row2);
    layoutCompactKnob (wetGainSlider, wetGainLabel, row2);
    layoutCompactKnob (stereoLinkSlider, stereoLinkLabel, row2);

    {
        auto modeCol = row2.removeFromLeft (modeW);
        stereoModeSelector.setBounds (modeCol.removeFromTop (compactKnobH).reduced (0, compactKnobH / 4));
        stereoModeLabel.setBounds (modeCol);
    }

    row2.removeFromLeft (6);

    // Group C: Input, Drive, Char
    layoutCompactKnob (inputGainSlider, inputGainLabel, row2);
    layoutCompactKnob (charDriveSlider, charDriveLabel, row2);

    {
        auto charCol = row2.removeFromLeft (charColW);
        auto charLblArea = charCol.removeFromLeft (22);
        charModeLabel.setBounds (charLblArea.withHeight (comboH).withCentre (charLblArea.getCentre()));
        charModeSelector.setBounds (charCol.withHeight (comboH).withCentre (charCol.getCentre()));
    }

    row2.removeFromLeft (6);

    // Remaining: OS, AutoThr, MeterScale, MIDI Trg, Pre
    {
        auto osLbl = row2.removeFromLeft (18);
        oversampleLabel.setBounds (osLbl.withHeight (comboH).withCentre (osLbl.getCentre()));
        auto osArea = row2.removeFromLeft (44);
        oversampleSelector.setBounds (osArea.withHeight (comboH).withCentre (osArea.getCentre()));
        row2.removeFromLeft (4);

        auto atArea = row2.removeFromLeft (52);
        autoThreshButton.setBounds (atArea.withHeight (btnH).withCentre (atArea.getCentre()));
        row2.removeFromLeft (4);

        auto mlbl = row2.removeFromLeft (28);
        meterScaleLabel.setBounds (mlbl.withHeight (comboH).withCentre (mlbl.getCentre()));
        auto mArea = row2.removeFromLeft (44);
        meterScaleSelector.setBounds (mArea.withHeight (comboH).withCentre (mArea.getCentre()));
        row2.removeFromLeft (4);

        auto mtArea = row2.removeFromLeft (52);
        midiTriggerButton.setBounds (mtArea.withHeight (btnH).withCentre (mtArea.getCentre()));
        row2.removeFromLeft (4);

        auto preArea = row2.removeFromLeft (28);
        analyzerPreButton.setBounds (preArea.withHeight (btnH).withCentre (preArea.getCentre()));
    }
}
