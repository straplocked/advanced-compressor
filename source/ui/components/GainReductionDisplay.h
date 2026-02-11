#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../LookAndFeel/ColourPalette.h"
#include "../../utils/Constants.h"

class GainReductionDisplay : public juce::Component
{
public:
    GainReductionDisplay();

    void paint (juce::Graphics& g) override;
    void setGainReduction (float grDB);
    void setLevels (float inputDB, float outputDB);

private:
    float gainReduction = 0.0f;
    float inputLevel = Constants::kMeterMinDB;
    float outputLevel = Constants::kMeterMinDB;
};
