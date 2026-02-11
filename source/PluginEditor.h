/*
 * Advanced Compressor - Open-source dynamic range compressor plugin
 * Copyright (C) 2024-2026 Advanced Compressor contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

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
