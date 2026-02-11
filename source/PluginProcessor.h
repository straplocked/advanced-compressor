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

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "dsp/CompressorEngine.h"
#include "analyzer/FFTProcessor.h"
#include "parameters/CompressorParameterAttachment.h"
#include "presets/PresetManager.h"
#include "midi/MIDILearnManager.h"
#include "utils/Constants.h"

class AdvancedCompressorProcessor : public juce::AudioProcessor
{
public:
    AdvancedCompressorProcessor();
    ~AdvancedCompressorProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override;
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    CompressorEngine& getCompressorEngine() { return compressorEngine; }
    FFTProcessor& getFFTProcessorPre()  { return fftProcessorPre; }
    FFTProcessor& getFFTProcessorPost() { return fftProcessorPost; }
    PresetManager& getPresetManager()   { return presetManager; }
    CompressorParameterAttachment& getParameterAttachment() { return paramAttachment; }
    juce::UndoManager& getUndoManager() { return undoManager; }
    MIDILearnManager& getMIDILearnManager() { return midiLearnManager; }

    // A/B comparison
    bool isStateA() const { return currentIsA; }
    void toggleAB();
    void copyAtoB();
    void copyBtoA();

private:
    juce::UndoManager undoManager { 30000, 30 };
    juce::AudioProcessorValueTreeState apvts;
    PresetManager presetManager;
    CompressorEngine compressorEngine;
    CompressorParameterAttachment paramAttachment;
    MIDILearnManager midiLearnManager;

    FFTProcessor fftProcessorPre;
    FFTProcessor fftProcessorPost;

    // A/B state storage
    bool currentIsA = true;
    juce::ValueTree stateA;
    juce::ValueTree stateB;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdvancedCompressorProcessor)
};
