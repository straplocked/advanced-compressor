#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../LookAndFeel/ColourPalette.h"
#include "../../utils/Constants.h"

class LevelMeterComponent : public juce::Component
{
public:
    LevelMeterComponent();

    void paint (juce::Graphics& g) override;

    void setLevels (float leftDB, float rightDB);
    void setPeakHold (float leftPeak, float rightPeak);

private:
    float levelToY (float dB) const;

    float leftLevel = Constants::kMeterMinDB;
    float rightLevel = Constants::kMeterMinDB;
    float leftPeak = Constants::kMeterMinDB;
    float rightPeak = Constants::kMeterMinDB;
};
