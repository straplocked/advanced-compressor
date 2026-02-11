#include "GainReductionDisplay.h"

GainReductionDisplay::GainReductionDisplay() = default;

void GainReductionDisplay::setGainReduction (float gr) { gainReduction = gr; repaint(); }
void GainReductionDisplay::setLevels (float inp, float out) { inputLevel = inp; outputLevel = out; repaint(); }

void GainReductionDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (2.0f);
    float w = bounds.getWidth();
    float h = bounds.getHeight();

    // Background
    g.setColour (ColourPalette::curveArea());
    g.fillRoundedRectangle (bounds, 4.0f);

    // GR bar (from top, growing downward)
    float grNormalized = juce::jlimit (0.0f, 1.0f, std::abs (gainReduction) / 40.0f);
    float grHeight = grNormalized * h;
    if (grHeight > 0.5f)
    {
        g.setColour (ColourPalette::gainReductionFill());
        g.fillRect (bounds.getX(), bounds.getY(), w, grHeight);

        g.setColour (ColourPalette::gainReduction());
        g.fillRect (bounds.getX(), bounds.getY() + grHeight - 2.0f, w, 2.0f);
    }

    // GR text
    g.setColour (ColourPalette::textBright());
    g.setFont (12.0f);
    juce::String grText = juce::String (gainReduction, 1) + " dB";
    g.drawText (grText, bounds, juce::Justification::centred);

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRoundedRectangle (bounds, 4.0f, 0.5f);
}
