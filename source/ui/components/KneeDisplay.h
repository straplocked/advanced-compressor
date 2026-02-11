#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../parameters/CompressorParameterAttachment.h"
#include "../LookAndFeel/ColourPalette.h"

class KneeDisplay : public juce::Component
{
public:
    KneeDisplay();

    void paint (juce::Graphics& g) override;
    void resized() override {}

    void updateParameters (float threshold, float ratio, float knee, float inputLevelDB);

private:
    float dbToX (float dB) const;
    float dbToY (float dB) const;

    float threshold = -20.0f;
    float ratio = 4.0f;
    float knee = 6.0f;
    float inputLevel = -60.0f;

    static constexpr float displayMinDB = -60.0f;
    static constexpr float displayMaxDB = 0.0f;
};
