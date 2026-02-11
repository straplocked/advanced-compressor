#include "PluginEditor.h"
#include "utils/Constants.h"
#include "ui/LookAndFeel/ColourPalette.h"

AdvancedCompressorEditor::AdvancedCompressorEditor (AdvancedCompressorProcessor& p)
    : AudioProcessorEditor (&p),
      headerToolbar (p),
      compressorDisplay (p),
      controlStrip (p)
{
    setLookAndFeel (&lookAndFeel);

    addAndMakeVisible (headerToolbar);
    addAndMakeVisible (compressorDisplay);
    addAndMakeVisible (controlStrip);

    setSize (Constants::kDefaultEditorWidth, Constants::kDefaultEditorHeight);
    setResizable (true, true);
    setResizeLimits (750, 450, 1920, 1080);
}

AdvancedCompressorEditor::~AdvancedCompressorEditor()
{
    setLookAndFeel (nullptr);
}

void AdvancedCompressorEditor::paint (juce::Graphics& g)
{
    g.fillAll (ColourPalette::background());
}

void AdvancedCompressorEditor::resized()
{
    auto bounds = getLocalBounds();

    headerToolbar.setBounds (bounds.removeFromTop (36));
    controlStrip.setBounds (bounds.removeFromBottom (110));
    compressorDisplay.setBounds (bounds);
}
