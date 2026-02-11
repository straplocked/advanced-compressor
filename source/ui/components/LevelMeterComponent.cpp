#include "LevelMeterComponent.h"

LevelMeterComponent::LevelMeterComponent() = default;

void LevelMeterComponent::setLevels (float l, float r)
{
    leftLevel = l;
    rightLevel = r;
    repaint();
}

void LevelMeterComponent::setPeakHold (float lp, float rp)
{
    leftPeak = lp;
    rightPeak = rp;
}

float LevelMeterComponent::levelToY (float dB) const
{
    float h = (float) getHeight();
    float normalized = (dB - Constants::kMeterMinDB) / (Constants::kMeterMaxDB - Constants::kMeterMinDB);
    return h * (1.0f - juce::jlimit (0.0f, 1.0f, normalized));
}

void LevelMeterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float meterWidth = (bounds.getWidth() - 2.0f) * 0.5f;

    // Background
    g.setColour (ColourPalette::curveArea());
    g.fillRoundedRectangle (bounds, 2.0f);

    auto drawMeter = [&] (float x, float dB, float peak)
    {
        float h = bounds.getHeight();
        float meterY = levelToY (dB);

        // Gradient meter bar
        juce::ColourGradient gradient (ColourPalette::meterGreen(), x, h,
                                        ColourPalette::meterRed(), x, 0.0f, false);
        gradient.addColour (0.6, ColourPalette::meterYellow());
        g.setGradientFill (gradient);
        g.fillRect (x, meterY, meterWidth, h - meterY);

        // Peak hold indicator
        if (peak > Constants::kMeterMinDB + 1.0f)
        {
            float peakY = levelToY (peak);
            g.setColour (ColourPalette::textBright());
            g.fillRect (x, peakY, meterWidth, 1.0f);
        }
    };

    drawMeter (bounds.getX(), leftLevel, leftPeak);
    drawMeter (bounds.getX() + meterWidth + 2.0f, rightLevel, rightPeak);

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRoundedRectangle (bounds, 2.0f, 0.5f);
}
