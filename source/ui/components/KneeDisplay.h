#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../parameters/CompressorParameterAttachment.h"
#include "../../dsp/CompressorStyle.h"
#include "../LookAndFeel/ColourPalette.h"
#include <deque>

class KneeDisplay : public juce::Component
{
public:
    KneeDisplay();

    void paint (juce::Graphics& g) override;
    void resized() override {}

    void updateParameters (float threshold, float ratio, float knee, float inputLevelDB);
    void setStyle (int styleIndex);

private:
    float threshold = -20.0f;
    float ratio = 4.0f;
    float knee = 6.0f;
    float inputLevel = -60.0f;
    int currentStyle = 0;

    // Animated trail of recent operating points
    struct TrailPoint
    {
        float inputDB;
        float outputDB;
        float age;  // 0.0 = newest, 1.0 = oldest
    };
    std::deque<TrailPoint> trail;
    static constexpr int kMaxTrailPoints = 40;
    static constexpr float kTrailAgeStep = 1.0f / (float) kMaxTrailPoints;

    static constexpr float displayMinDB = -60.0f;
    static constexpr float displayMaxDB = 0.0f;
};
