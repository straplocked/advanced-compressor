#pragma once

#include "PluginProcessor.h"
#include "ui/components/CompressorDisplay.h"
#include "ui/components/BottomControlStrip.h"
#include "ui/components/HeaderToolbar.h"
#include "ui/LookAndFeel/AdvancedCompressorLookAndFeel.h"
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_opengl/juce_opengl.h>

class AdvancedCompressorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AdvancedCompressorEditor (AdvancedCompressorProcessor&);
    ~AdvancedCompressorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AdvancedCompressorLookAndFeel lookAndFeel;

    HeaderToolbar headerToolbar;
    CompressorDisplay compressorDisplay;
    BottomControlStrip controlStrip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdvancedCompressorEditor)
};
