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

#include "CompressorDisplay.h"
#include "../../PluginProcessor.h"
#include "../LookAndFeel/Fonts.h"

CompressorDisplay::CompressorDisplay (AdvancedCompressorProcessor& p)
    : processor (p)
{
    addAndMakeVisible (kneeDisplay);

    inputMeter.setLabel ("IN");
    addAndMakeVisible (inputMeter);

    outputMeter.setLabel ("OUT");
    addAndMakeVisible (outputMeter);

    addAndMakeVisible (grDisplay);
    addAndMakeVisible (grHistory);

    spectrumPre.prepare (Constants::kCurveResolution);
    spectrumPost.prepare (Constants::kCurveResolution);

    loudnessLabel.setFont (Fonts::getBody (11.0f));
    loudnessLabel.setColour (juce::Label::textColourId, ColourPalette::axisText());
    loudnessLabel.setJustificationType (juce::Justification::centredRight);
    addAndMakeVisible (loudnessLabel);

    startTimerHz (Constants::kUITimerHz);
}

CompressorDisplay::~CompressorDisplay()
{
    stopTimer();
}

void CompressorDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour (ColourPalette::curveArea());
    g.fillRect (bounds);

    // Spectrum backdrop (within knee display area, excluding right meter cluster)
    auto specBounds = bounds.withTrimmedRight (80.0f).reduced (4.0f, 0.0f);

    g.saveState();
    g.setOrigin ((int) specBounds.getX(), (int) specBounds.getY());

    // Pre spectrum
    g.setColour (ColourPalette::spectrumPre());
    g.fillPath (spectrumPre.getFillPath());

    // Post spectrum
    g.setColour (ColourPalette::spectrumPost());
    g.fillPath (spectrumPost.getFillPath());
    g.setColour (ColourPalette::spectrumPost().withAlpha (0.8f));
    g.strokePath (spectrumPost.getPath(), juce::PathStrokeType (1.0f));

    g.restoreState();

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRect (bounds, 1.0f);
}

void CompressorDisplay::resized()
{
    auto bounds = getLocalBounds();

    // Right-side meter cluster (Pro-C 3 style): GR | IN | OUT
    auto meterArea = bounds.removeFromRight (80);

    // Loudness readout at the top of the meter area
    loudnessLabel.setBounds (meterArea.removeFromTop (40).reduced (2, 2));

    // Meter bars fill the remaining height
    auto grArea = meterArea.removeFromLeft (24);
    grDisplay.setBounds (grArea.reduced (2));

    auto inArea = meterArea.removeFromLeft (26);
    inputMeter.setBounds (inArea.reduced (2));

    outputMeter.setBounds (meterArea.reduced (2));

    // Knee display fills the entire main area
    kneeDisplay.setBounds (bounds.reduced (4));

    // GR history graph: bottom 30% of the main display area (overlaid on knee/spectrum)
    auto histBounds = bounds.reduced (4);
    histBounds = histBounds.removeFromBottom (histBounds.getHeight() * 3 / 10);
    grHistory.setBounds (histBounds);
    grHistory.toFront (false);
}

void CompressorDisplay::timerCallback()
{
    // Update spectrum (same area as paint)
    auto specBounds = getLocalBounds().withTrimmedRight (80).reduced (4, 0);
    float specW = (float) specBounds.getWidth();
    float specH = (float) specBounds.getHeight();

    spectrumPre.update (processor.getFFTProcessorPre(), specW, specH);
    spectrumPost.update (processor.getFFTProcessorPost(), specW, specH);

    // Update meters from triple buffer
    auto& meterData = processor.getCompressorEngine().getMeterData();
    auto& meterBuf = meterData.getBuffer();

    if (meterBuf.swapRead())
    {
        const auto* frame = meterBuf.getReadPointer();

        inputMeter.setLevels (frame->inputL, frame->inputR);
        outputMeter.setLevels (frame->outputL, frame->outputR);
        grDisplay.setGainReduction (std::min (frame->grL, frame->grR));
        grHistory.pushGainReduction (std::min (frame->grL, frame->grR));
        grDisplay.setLevels (std::max (frame->inputL, frame->inputR),
                             std::max (frame->outputL, frame->outputR));

        // Update knee display with current operating point
        auto params = processor.getParameterAttachment().getCurrentParameters();
        float inputDB = std::max (frame->inputL, frame->inputR);
        kneeDisplay.setStyle (params.style);
        kneeDisplay.updateParameters (params.threshold, params.ratio, params.knee, inputDB);

        // Update loudness readout
        auto formatLUFS = [] (float lufs) -> juce::String
        {
            if (lufs <= -99.0f) return "-inf";
            return juce::String (lufs, 1);
        };
        juce::String loudText;
        loudText += "M: " + formatLUFS (frame->momentaryLUFS) + " LUFS\n";
        loudText += "S: " + formatLUFS (frame->shortTermLUFS) + " LUFS\n";
        loudText += "I: " + formatLUFS (frame->integratedLUFS) + " LUFS";
        loudnessLabel.setText (loudText, juce::dontSendNotification);
    }

    repaint();
}
