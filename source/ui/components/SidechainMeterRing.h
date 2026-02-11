#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../LookAndFeel/ColourPalette.h"

class SidechainMeterRing : public juce::Component
{
public:
    SidechainMeterRing();
    void paint (juce::Graphics& g) override;
    void setGainReduction (float grDB);

private:
    float gainReduction = 0.0f;
};
