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

#include "HeaderToolbar.h"
#include "../../PluginProcessor.h"
#include "../../parameters/ParameterIDs.h"
#include "../../midi/MIDILearnManager.h"

HeaderToolbar::HeaderToolbar (AdvancedCompressorProcessor& p)
    : processor (p),
      presetManager (p.getPresetManager())
{
    presetManager.addListener (this);
    auto& apvts = processor.getAPVTS();

    // Preset controls
    prevButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    prevButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    prevButton.onClick = [this] { presetManager.previousPreset(); };
    addAndMakeVisible (prevButton);

    nextButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    nextButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    nextButton.onClick = [this] { presetManager.nextPreset(); };
    addAndMakeVisible (nextButton);

    presetNameButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetNameButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textBright());
    presetNameButton.onClick = [this] { showPresetMenu(); };
    addAndMakeVisible (presetNameButton);

    saveButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    saveButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    saveButton.onClick = [this] { showSaveDialog(); };
    addAndMakeVisible (saveButton);

    updatePresetName();

    // Undo / Redo
    undoButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    undoButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    undoButton.onClick = [this] { processor.getUndoManager().undo(); };
    addAndMakeVisible (undoButton);

    redoButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    redoButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    redoButton.onClick = [this] { processor.getUndoManager().redo(); };
    addAndMakeVisible (redoButton);

    // A/B comparison
    abButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    abButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textBright());
    abButton.onClick = [this]
    {
        processor.toggleAB();
        abButton.setButtonText (processor.isStateA() ? "A" : "B");
    };
    addAndMakeVisible (abButton);

    copyABButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    copyABButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    copyABButton.onClick = [this] { processor.copyAtoB(); };
    addAndMakeVisible (copyABButton);

    // MIDI Learn toggle
    midiLearnButton.setClickingTogglesState (true);
    midiLearnButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    midiLearnButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    midiLearnButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x40FF4444));
    midiLearnButton.onClick = [this]
    {
        auto& learnMgr = processor.getMIDILearnManager();
        if (midiLearnButton.getToggleState())
        {
            // Show popup to pick a parameter to learn
            juce::PopupMenu menu;
            auto styleNames = ParamIDs::getStyleNames();
            // List common automatable parameters
            struct ParamEntry { const char* id; const char* name; };
            static const ParamEntry entries[] = {
                { ParamIDs::kThreshold,  "Threshold" },
                { ParamIDs::kRatio,      "Ratio" },
                { ParamIDs::kKnee,       "Knee" },
                { ParamIDs::kRange,      "Range" },
                { ParamIDs::kAttack,     "Attack" },
                { ParamIDs::kRelease,    "Release" },
                { ParamIDs::kHold,       "Hold" },
                { ParamIDs::kInputGain,  "Input Gain" },
                { ParamIDs::kOutputGain, "Output Gain" },
                { ParamIDs::kWetGain,    "Wet Gain" },
                { ParamIDs::kMix,        "Mix" },
                { ParamIDs::kCharDrive,  "Drive" },
                { ParamIDs::kStereoLink, "Stereo Link" },
            };
            for (int i = 0; i < 13; ++i)
                menu.addItem (i + 1, entries[i].name);
            menu.addSeparator();
            menu.addItem (100, "Clear All Mappings");

            menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&midiLearnButton),
                [this, &learnMgr] (int result)
                {
                    if (result >= 1 && result <= 13)
                    {
                        static const char* paramIDs[] = {
                            ParamIDs::kThreshold, ParamIDs::kRatio, ParamIDs::kKnee,
                            ParamIDs::kRange, ParamIDs::kAttack, ParamIDs::kRelease,
                            ParamIDs::kHold, ParamIDs::kInputGain, ParamIDs::kOutputGain,
                            ParamIDs::kWetGain, ParamIDs::kMix, ParamIDs::kCharDrive,
                            ParamIDs::kStereoLink
                        };
                        learnMgr.startLearning (paramIDs[result - 1]);
                    }
                    else if (result == 100)
                    {
                        learnMgr.clearAll();
                    }
                    else
                    {
                        midiLearnButton.setToggleState (false, juce::dontSendNotification);
                    }
                });
        }
        else
        {
            learnMgr.stopLearning();
        }
    };
    addAndMakeVisible (midiLearnButton);

    // Delta listen mode
    deltaButton.setClickingTogglesState (true);
    deltaButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    deltaButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    deltaButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::gainReduction().withAlpha (0.3f));
    addAndMakeVisible (deltaButton);
    deltaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kDelta, deltaButton);

    // Style selector
    styleSelector.addItemList (ParamIDs::getStyleNames(), 1);
    styleSelector.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (styleSelector);
    styleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        apvts, ParamIDs::kCompStyle, styleSelector);

    // Oversample selector
    oversampleSelector.addItemList (ParamIDs::getOversamplingNames(), 1);
    oversampleSelector.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (oversampleSelector);
    oversampleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        apvts, ParamIDs::kOversample, oversampleSelector);

    // Combo box labels
    auto setupComboLabel = [this] (juce::Label& label, const juce::String& text)
    {
        label.setText (text, juce::dontSendNotification);
        label.setFont (9.0f);
        label.setColour (juce::Label::textColourId, ColourPalette::axisText());
        label.setJustificationType (juce::Justification::centredRight);
        addAndMakeVisible (label);
    };
    setupComboLabel (styleLabel, "Style");
    setupComboLabel (oversampleLabel, "OS");
    setupComboLabel (scaleLabel, "UI");

    // Scale selector
    scaleSelector.addItemList ({ "75%", "100%", "125%", "150%", "200%" }, 1);
    scaleSelector.setSelectedId (2, juce::dontSendNotification); // default 100%
    scaleSelector.setJustificationType (juce::Justification::centred);
    scaleSelector.onChange = [this]
    {
        static constexpr float scaleFactors[] = { 0.75f, 1.0f, 1.25f, 1.5f, 2.0f };
        int idx = scaleSelector.getSelectedId() - 1;
        if (idx >= 0 && idx < 5)
        {
            if (auto* editor = dynamic_cast<juce::AudioProcessorEditor*> (getParentComponent()))
                editor->setScaleFactor (scaleFactors[idx]);
        }
    };
    addAndMakeVisible (scaleSelector);

    // Analyzer toggle
    analyzerButton.setClickingTogglesState (true);
    analyzerButton.setColour (juce::TextButton::buttonOnColourId, ColourPalette::spectrumPost().withAlpha (0.3f));
    addAndMakeVisible (analyzerButton);
    analyzerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kAnalyzerOn, analyzerButton);

    // Output gain knob
    outputGainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible (outputGainSlider);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        apvts, ParamIDs::kOutputGain, outputGainSlider);

    outputGainLabel.setText ("Out", juce::dontSendNotification);
    outputGainLabel.setJustificationType (juce::Justification::centred);
    outputGainLabel.setFont (10.0f);
    outputGainLabel.setColour (juce::Label::textColourId, ColourPalette::axisText());
    addAndMakeVisible (outputGainLabel);
}

HeaderToolbar::~HeaderToolbar()
{
    presetManager.removeListener (this);
    deltaAttachment.reset();
    styleAttachment.reset();
    oversampleAttachment.reset();
    analyzerAttachment.reset();
    outputGainAttachment.reset();
}

void HeaderToolbar::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient grad (ColourPalette::panelTop(), 0, bounds.getY(),
                               ColourPalette::panelBottom(), 0, bounds.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRect (bounds);

    float bottomY = bounds.getBottom();
    g.setColour (ColourPalette::bevelLight());
    g.drawLine (0.0f, bottomY - 1.5f, bounds.getRight(), bottomY - 1.5f, 1.0f);
    g.setColour (ColourPalette::bevelDark());
    g.drawLine (0.0f, bottomY - 0.5f, bounds.getRight(), bottomY - 0.5f, 1.0f);

    g.setFont (16.0f);
    g.setColour (juce::Colour (0x40000000));
    g.drawText ("Advanced Compressor", 13, 1, 170, getHeight(), juce::Justification::centredLeft, true);
    g.setColour (ColourPalette::textBright());
    g.drawText ("Advanced Compressor", 12, 0, 170, getHeight(), juce::Justification::centredLeft, true);
}

void HeaderToolbar::resized()
{
    auto bounds = getLocalBounds().reduced (8, 4);
    bounds.removeFromLeft (176);

    auto rightBounds = bounds;

    auto outArea = rightBounds.removeFromRight (50);
    auto outKnob = outArea.removeFromTop (outArea.getHeight() - 2);
    outputGainSlider.setBounds (outKnob.withSizeKeepingCentre (28, 28));
    outputGainLabel.setBounds (outArea);
    rightBounds.removeFromRight (6);

    auto scaleLabelArea = rightBounds.removeFromRight (18);
    scaleLabel.setBounds (scaleLabelArea.withHeight (22).withCentre (scaleLabelArea.getCentre()));
    auto scaleArea = rightBounds.removeFromRight (48);
    scaleSelector.setBounds (scaleArea.withHeight (22).withCentre (scaleArea.getCentre()));
    rightBounds.removeFromRight (4);

    auto osLabelArea = rightBounds.removeFromRight (18);
    oversampleLabel.setBounds (osLabelArea.withHeight (22).withCentre (osLabelArea.getCentre()));
    auto osArea = rightBounds.removeFromRight (50);
    oversampleSelector.setBounds (osArea.withHeight (22).withCentre (osArea.getCentre()));
    rightBounds.removeFromRight (6);

    auto styleLabelArea = rightBounds.removeFromRight (30);
    styleLabel.setBounds (styleLabelArea.withHeight (22).withCentre (styleLabelArea.getCentre()));
    auto styleArea = rightBounds.removeFromRight (90);
    styleSelector.setBounds (styleArea.withHeight (22).withCentre (styleArea.getCentre()));
    rightBounds.removeFromRight (6);

    auto analyzerArea = rightBounds.removeFromRight (62);
    analyzerButton.setBounds (analyzerArea.withHeight (22).withCentre (analyzerArea.getCentre()));
    rightBounds.removeFromRight (4);

    // Delta button
    auto deltaArea = rightBounds.removeFromRight (42);
    deltaButton.setBounds (deltaArea.withHeight (22).withCentre (deltaArea.getCentre()));
    rightBounds.removeFromRight (4);

    // A/B buttons
    auto copyABArea = rightBounds.removeFromRight (30);
    copyABButton.setBounds (copyABArea.withHeight (22).withCentre (copyABArea.getCentre()));
    rightBounds.removeFromRight (2);

    auto abArea = rightBounds.removeFromRight (22);
    abButton.setBounds (abArea.withHeight (22).withCentre (abArea.getCentre()));
    rightBounds.removeFromRight (4);

    // MIDI Learn
    auto midiArea = rightBounds.removeFromRight (36);
    midiLearnButton.setBounds (midiArea.withHeight (22).withCentre (midiArea.getCentre()));
    rightBounds.removeFromRight (4);

    // Undo / Redo
    auto redoArea = rightBounds.removeFromRight (36);
    redoButton.setBounds (redoArea.withHeight (22).withCentre (redoArea.getCentre()));
    rightBounds.removeFromRight (2);

    auto undoArea = rightBounds.removeFromRight (36);
    undoButton.setBounds (undoArea.withHeight (22).withCentre (undoArea.getCentre()));
    rightBounds.removeFromRight (4);

    auto presetBounds = rightBounds;
    presetBounds.removeFromRight (8);

    auto presetArea = presetBounds.withSizeKeepingCentre (
        juce::jmin (280, presetBounds.getWidth()), presetBounds.getHeight());

    prevButton.setBounds (presetArea.removeFromLeft (24).withHeight (22).withCentre (
        juce::Point<int> (presetArea.getX() - 12, presetArea.getCentreY())));

    auto saveBtnArea = presetArea.removeFromRight (38);
    saveButton.setBounds (saveBtnArea.withHeight (22).withCentre (saveBtnArea.getCentre()));
    presetArea.removeFromRight (4);

    auto nextBtnArea = presetArea.removeFromRight (24);
    nextButton.setBounds (nextBtnArea.withHeight (22).withCentre (nextBtnArea.getCentre()));

    presetArea.removeFromLeft (4);
    presetArea.removeFromRight (4);
    presetNameButton.setBounds (presetArea.withHeight (22).withCentre (presetArea.getCentre()));
}

void HeaderToolbar::presetChanged() { updatePresetName(); }
void HeaderToolbar::updatePresetName() { presetNameButton.setButtonText (presetManager.getCurrentPresetName()); }

void HeaderToolbar::showPresetMenu()
{
    juce::PopupMenu menu;
    auto categories = presetManager.getCategories();
    auto& allPresets = presetManager.getAllPresets();

    // Favorites section
    auto favIndices = presetManager.getFavoriteIndices();
    if (! favIndices.empty())
    {
        juce::PopupMenu favMenu;
        for (int i : favIndices)
        {
            bool isCurrent = (i == presetManager.getCurrentPresetIndex());
            favMenu.addItem (i + 1, allPresets[(size_t) i].name, true, isCurrent);
        }
        menu.addSubMenu ("Favorites", favMenu);
        menu.addSeparator();
    }

    // Category sections
    for (auto& category : categories)
    {
        juce::PopupMenu subMenu;
        for (int i = 0; i < (int) allPresets.size(); ++i)
        {
            if (allPresets[(size_t) i].category == category)
            {
                bool isCurrent = (i == presetManager.getCurrentPresetIndex());
                juce::String label = allPresets[(size_t) i].name;
                if (allPresets[(size_t) i].isFavorite)
                    label = "* " + label;
                subMenu.addItem (i + 1, label, true, isCurrent);
            }
        }
        menu.addSubMenu (category, subMenu);
    }

    menu.addSeparator();
    menu.addItem (-1, "Save Preset...");

    auto idx = presetManager.getCurrentPresetIndex();
    if (idx >= 0 && idx < (int) allPresets.size())
    {
        juce::String favLabel = allPresets[(size_t) idx].isFavorite
            ? "Remove from Favorites" : "Add to Favorites";
        menu.addItem (-3, favLabel);
    }

    if (idx >= 0 && idx < (int) allPresets.size() && ! allPresets[(size_t) idx].isFactory)
        menu.addItem (-2, "Delete \"" + allPresets[(size_t) idx].name + "\"");

    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&presetNameButton),
        [this] (int result)
        {
            if (result == -1)
                showSaveDialog();
            else if (result == -2)
                presetManager.deleteUserPreset (presetManager.getCurrentPresetIndex());
            else if (result == -3)
                presetManager.toggleFavorite (presetManager.getCurrentPresetIndex());
            else if (result > 0)
                presetManager.applyPreset (result - 1);
        });
}

void HeaderToolbar::showSaveDialog()
{
    auto* nameEditor = new juce::AlertWindow ("Save Preset", "Enter a name for your preset:", juce::MessageBoxIconType::NoIcon);
    nameEditor->addTextEditor ("name", presetManager.getCurrentPresetName(), "Preset name:");
    nameEditor->addButton ("Save", 1);
    nameEditor->addButton ("Cancel", 0);

    nameEditor->enterModalState (true, juce::ModalCallbackFunction::create (
        [this, nameEditor] (int result)
        {
            if (result == 1)
            {
                auto name = nameEditor->getTextEditorContents ("name");
                presetManager.saveCurrentAsPreset (name);
            }
            delete nameEditor;
        }), false);
}
