#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../../presets/PresetManager.h"
#include "../LookAndFeel/ColourPalette.h"

class AdvancedCompressorProcessor;

class HeaderToolbar : public juce::Component,
                      public PresetManager::Listener
{
public:
    explicit HeaderToolbar (AdvancedCompressorProcessor& processor);
    ~HeaderToolbar() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void presetChanged() override;

private:
    AdvancedCompressorProcessor& processor;
    PresetManager& presetManager;

    juce::TextButton prevButton { "<" };
    juce::TextButton nextButton { ">" };
    juce::TextButton presetNameButton { "Init" };
    juce::TextButton saveButton { "Save" };

    juce::TextButton undoButton { "Undo" };
    juce::TextButton redoButton { "Redo" };
    juce::TextButton deltaButton { "Delta" };
    juce::TextButton abButton { "A" };
    juce::TextButton copyABButton { "A>B" };
    juce::TextButton midiLearnButton { "MIDI" };

    juce::ComboBox styleSelector;
    juce::ComboBox oversampleSelector;
    juce::ComboBox scaleSelector;
    juce::TextButton analyzerButton { "Analyzer" };
    juce::Slider outputGainSlider;
    juce::Label outputGainLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> styleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oversampleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> analyzerAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> deltaAttachment;

    void showPresetMenu();
    void showSaveDialog();
    void updatePresetName();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderToolbar)
};
