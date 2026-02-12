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
#include "../LookAndFeel/Fonts.h"
#include "../LookAndFeel/Icons.h"

HeaderToolbar::HeaderToolbar (AdvancedCompressorProcessor& p)
    : processor (p),
      presetManager (p.getPresetManager()),
      prevButton ("Prev", Icons::getChevronLeft()),
      nextButton ("Next", Icons::getChevronRight()),
      saveButton ("Save", Icons::getSave()),
      undoButton ("Undo", Icons::getUndo()),
      redoButton ("Redo", Icons::getRedo()),
      deltaButton ("Delta", Icons::getDelta())
{
    presetManager.addListener (this);
    auto& apvts = processor.getAPVTS();

    // Preset navigation
    prevButton.setComponentID ("header.prevPreset");
    prevButton.onClick = [this] { presetManager.previousPreset(); };
    addAndMakeVisible (prevButton);

    nextButton.setComponentID ("header.nextPreset");
    nextButton.onClick = [this] { presetManager.nextPreset(); };
    addAndMakeVisible (nextButton);

    presetNameButton.setComponentID ("header.presetName");
    presetNameButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    presetNameButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textBright());
    presetNameButton.onClick = [this] { showPresetMenu(); };
    addAndMakeVisible (presetNameButton);

    saveButton.setComponentID ("header.savePreset");
    saveButton.onClick = [this] { showSaveDialog(); };
    addAndMakeVisible (saveButton);

    updatePresetName();

    // Undo / Redo
    undoButton.setComponentID ("header.undo");
    undoButton.onClick = [this] { processor.getUndoManager().undo(); };
    addAndMakeVisible (undoButton);

    redoButton.setComponentID ("header.redo");
    redoButton.onClick = [this] { processor.getUndoManager().redo(); };
    addAndMakeVisible (redoButton);

    // A/B comparison
    abButton.setComponentID ("header.abToggle");
    abButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    abButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textBright());
    abButton.onClick = [this]
    {
        processor.toggleAB();
        abButton.setButtonText (processor.isStateA() ? "A" : "B");
    };
    addAndMakeVisible (abButton);

    copyABButton.setComponentID ("header.copyAB");
    copyABButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    copyABButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    copyABButton.onClick = [this] { processor.copyAtoB(); };
    addAndMakeVisible (copyABButton);

    // Delta listen mode
    deltaButton.setComponentID ("header.delta");
    deltaButton.setClickingTogglesState (true);
    deltaButton.setActiveColour (ColourPalette::gainReduction());
    addAndMakeVisible (deltaButton);
    deltaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        apvts, ParamIDs::kDelta, deltaButton);

    // MIDI Learn toggle
    midiLearnButton.setComponentID ("header.midiLearn");
    midiLearnButton.setClickingTogglesState (true);
    midiLearnButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    midiLearnButton.setColour (juce::TextButton::textColourOffId, ColourPalette::textDim());
    midiLearnButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0x40FF4444));
    midiLearnButton.onClick = [this]
    {
        auto& learnMgr = processor.getMIDILearnManager();
        if (midiLearnButton.getToggleState())
        {
            juce::PopupMenu menu;
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

}

HeaderToolbar::~HeaderToolbar()
{
    presetManager.removeListener (this);
    deltaAttachment.reset();
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

    g.setFont (Fonts::getTitle (18.0f));
    g.setColour (juce::Colour (0x40000000));
    g.drawText ("ADVANCED COMPRESSOR", 13, 1, 250, getHeight(), juce::Justification::centredLeft, true);
    g.setColour (ColourPalette::textBright());
    g.drawText ("ADVANCED COMPRESSOR", 12, 0, 250, getHeight(), juce::Justification::centredLeft, true);
}

void HeaderToolbar::resized()
{
    auto bounds = getLocalBounds().reduced (8, 4);
    bounds.removeFromLeft (260);

    int iconSize = 28;
    int textBtnH = 22;

    auto rightBounds = bounds;

    // Right side: MIDI Learn
    auto midiArea = rightBounds.removeFromRight (36);
    midiLearnButton.setBounds (midiArea.withHeight (textBtnH).withCentre (midiArea.getCentre()));
    rightBounds.removeFromRight (6);

    // Delta
    auto deltaArea = rightBounds.removeFromRight (iconSize);
    deltaButton.setBounds (deltaArea.withSizeKeepingCentre (iconSize, iconSize));
    rightBounds.removeFromRight (6);

    // A/B buttons
    auto copyABArea = rightBounds.removeFromRight (30);
    copyABButton.setBounds (copyABArea.withHeight (textBtnH).withCentre (copyABArea.getCentre()));
    rightBounds.removeFromRight (2);

    auto abArea = rightBounds.removeFromRight (22);
    abButton.setBounds (abArea.withHeight (textBtnH).withCentre (abArea.getCentre()));
    rightBounds.removeFromRight (6);

    // Undo / Redo
    auto redoArea = rightBounds.removeFromRight (iconSize);
    redoButton.setBounds (redoArea.withSizeKeepingCentre (iconSize, iconSize));
    rightBounds.removeFromRight (2);

    auto undoArea = rightBounds.removeFromRight (iconSize);
    undoButton.setBounds (undoArea.withSizeKeepingCentre (iconSize, iconSize));
    rightBounds.removeFromRight (6);

    // Save
    auto saveArea = rightBounds.removeFromRight (iconSize);
    saveButton.setBounds (saveArea.withSizeKeepingCentre (iconSize, iconSize));
    rightBounds.removeFromRight (4);

    // Preset name + navigation
    auto presetBounds = rightBounds;
    presetBounds.removeFromRight (4);

    auto presetArea = presetBounds.withSizeKeepingCentre (
        juce::jmin (240, presetBounds.getWidth()), presetBounds.getHeight());

    auto nextBtnArea = presetArea.removeFromRight (iconSize);
    nextButton.setBounds (nextBtnArea.withSizeKeepingCentre (iconSize, iconSize));

    auto prevBtnArea = presetArea.removeFromLeft (iconSize);
    prevButton.setBounds (prevBtnArea.withSizeKeepingCentre (iconSize, iconSize));

    presetArea.removeFromLeft (2);
    presetArea.removeFromRight (2);
    presetNameButton.setBounds (presetArea.withHeight (textBtnH).withCentre (presetArea.getCentre()));
}

void HeaderToolbar::presetChanged() { updatePresetName(); }
void HeaderToolbar::updatePresetName() { presetNameButton.setButtonText (presetManager.getCurrentPresetName()); }

void HeaderToolbar::showPresetMenu()
{
    juce::PopupMenu menu;
    auto categories = presetManager.getCategories();
    auto& allPresets = presetManager.getAllPresets();

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
