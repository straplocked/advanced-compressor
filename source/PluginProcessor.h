#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "dsp/CompressorEngine.h"
#include "analyzer/FFTProcessor.h"
#include "parameters/CompressorParameterAttachment.h"
#include "presets/PresetManager.h"
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
    bool acceptsMidi() const override { return false; }
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

private:
    juce::AudioProcessorValueTreeState apvts;
    PresetManager presetManager;
    CompressorEngine compressorEngine;
    CompressorParameterAttachment paramAttachment;

    FFTProcessor fftProcessorPre;
    FFTProcessor fftProcessorPost;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdvancedCompressorProcessor)
};
