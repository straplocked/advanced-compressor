#include "KneeDisplay.h"
#include "../../utils/CompressorMath.h"

KneeDisplay::KneeDisplay() = default;

void KneeDisplay::updateParameters (float thresh, float rat, float kn, float inputDB)
{
    threshold = thresh;
    ratio = rat;
    knee = kn;
    inputLevel = inputDB;
    repaint();
}

void KneeDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (4.0f);
    float w = bounds.getWidth();
    float h = bounds.getHeight();

    // Background
    g.setColour (ColourPalette::curveArea());
    g.fillRoundedRectangle (bounds, 4.0f);

    // Grid lines
    g.setColour (ColourPalette::gridLine());
    for (float dB = -50.0f; dB <= 0.0f; dB += 10.0f)
    {
        float x = bounds.getX() + (dB - displayMinDB) / (displayMaxDB - displayMinDB) * w;
        float y = bounds.getY() + h - (dB - displayMinDB) / (displayMaxDB - displayMinDB) * h;
        g.drawVerticalLine ((int) x, bounds.getY(), bounds.getBottom());
        g.drawHorizontalLine ((int) y, bounds.getX(), bounds.getRight());
    }

    // 1:1 line (unity)
    g.setColour (ColourPalette::gridLineStrong());
    g.drawLine (bounds.getX(), bounds.getBottom(), bounds.getRight(), bounds.getY(), 0.5f);

    // Threshold line
    g.setColour (ColourPalette::thresholdLine().withAlpha (0.4f));
    float threshX = bounds.getX() + (threshold - displayMinDB) / (displayMaxDB - displayMinDB) * w;
    g.drawVerticalLine ((int) threshX, bounds.getY(), bounds.getBottom());

    // Knee region shading
    if (knee > 0.01f)
    {
        float halfKnee = knee * 0.5f;
        float kneeStartX = bounds.getX() + (threshold - halfKnee - displayMinDB) / (displayMaxDB - displayMinDB) * w;
        float kneeEndX = bounds.getX() + (threshold + halfKnee - displayMinDB) / (displayMaxDB - displayMinDB) * w;
        g.setColour (ColourPalette::kneeRegion());
        g.fillRect (kneeStartX, bounds.getY(), kneeEndX - kneeStartX, h);
    }

    // Transfer curve
    juce::Path curvePath;
    bool started = false;
    for (float inputDB = displayMinDB; inputDB <= displayMaxDB; inputDB += 0.5f)
    {
        float gr = CompressorMath::computeGainReduction (inputDB, threshold, ratio, knee, 60.0f);
        float outputDB = inputDB + gr;

        float x = bounds.getX() + (inputDB - displayMinDB) / (displayMaxDB - displayMinDB) * w;
        float y = bounds.getY() + h - (outputDB - displayMinDB) / (displayMaxDB - displayMinDB) * h;

        y = juce::jlimit (bounds.getY(), bounds.getBottom(), y);

        if (!started) { curvePath.startNewSubPath (x, y); started = true; }
        else curvePath.lineTo (x, y);
    }

    g.setColour (ColourPalette::transferCurve());
    g.strokePath (curvePath, juce::PathStrokeType (2.0f));

    // Operating point
    if (inputLevel > displayMinDB)
    {
        float gr = CompressorMath::computeGainReduction (inputLevel, threshold, ratio, knee, 60.0f);
        float outputDB = inputLevel + gr;

        float opX = bounds.getX() + (inputLevel - displayMinDB) / (displayMaxDB - displayMinDB) * w;
        float opY = bounds.getY() + h - (outputDB - displayMinDB) / (displayMaxDB - displayMinDB) * h;

        opX = juce::jlimit (bounds.getX(), bounds.getRight(), opX);
        opY = juce::jlimit (bounds.getY(), bounds.getBottom(), opY);

        g.setColour (ColourPalette::operatingPoint());
        g.fillEllipse (opX - 4.0f, opY - 4.0f, 8.0f, 8.0f);
        g.setColour (ColourPalette::operatingPoint().withAlpha (0.3f));
        g.fillEllipse (opX - 8.0f, opY - 8.0f, 16.0f, 16.0f);
    }

    // Border
    g.setColour (ColourPalette::gridLine());
    g.drawRoundedRectangle (bounds, 4.0f, 1.0f);
}
