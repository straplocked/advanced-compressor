#include "CompressorDisplay.h"
#include "../../PluginProcessor.h"

CompressorDisplay::CompressorDisplay (AdvancedCompressorProcessor& p)
    : processor (p)
{
    addAndMakeVisible (kneeDisplay);
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);
    addAndMakeVisible (grDisplay);

    spectrumPre.prepare (Constants::kCurveResolution);
    spectrumPost.prepare (Constants::kCurveResolution);

    loudnessLabel.setFont (10.0f);
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

    // Spectrum backdrop
    auto specBounds = bounds.reduced (40.0f, 0.0f);
    float specW = specBounds.getWidth();
    float specH = specBounds.getHeight();

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

    auto leftMeter = bounds.removeFromLeft (30);
    inputMeter.setBounds (leftMeter.reduced (4));

    auto rightMeter = bounds.removeFromRight (30);
    outputMeter.setBounds (rightMeter.reduced (4));

    // Loudness readout at top right
    loudnessLabel.setBounds (bounds.getRight() - 190, bounds.getY() + 2, 186, 36);

    // Split remaining area: left half = knee, right half = GR display
    auto leftHalf = bounds.removeFromLeft (bounds.getWidth() / 2);
    kneeDisplay.setBounds (leftHalf.reduced (4));
    grDisplay.setBounds (bounds.reduced (4));
}

void CompressorDisplay::timerCallback()
{
    // Update spectrum
    auto specBounds = getLocalBounds().reduced (40, 0);
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
