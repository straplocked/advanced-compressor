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

#include <juce_graphics/juce_graphics.h>

namespace ColourPalette
{
    // Background / Grid
    inline juce::Colour background()       { return juce::Colour (0xff121228); }
    inline juce::Colour curveArea()         { return juce::Colour (0xff1a1a36); }
    inline juce::Colour gridLine()          { return juce::Colour (0xff2a2a4a); }
    inline juce::Colour gridLineStrong()    { return juce::Colour (0xff3a3a5a); }
    inline juce::Colour axisText()          { return juce::Colour (0xff888899); }

    // Compressor-specific colors
    inline juce::Colour gainReduction()     { return juce::Colour (0xffff4444); }
    inline juce::Colour gainReductionFill() { return juce::Colour (0x40ff4444); }
    inline juce::Colour thresholdLine()     { return juce::Colour (0xffffd93d); }
    inline juce::Colour thresholdAccent()   { return juce::Colour (0xffffa726); }
    inline juce::Colour meterGreen()        { return juce::Colour (0xff4caf50); }
    inline juce::Colour meterYellow()       { return juce::Colour (0xffffeb3b); }
    inline juce::Colour meterRed()          { return juce::Colour (0xffff5252); }
    inline juce::Colour kneeRegion()        { return juce::Colour (0x20ffd93d); }
    inline juce::Colour operatingPoint()    { return juce::Colour (0xffffffff); }
    inline juce::Colour transferCurve()     { return juce::Colour (0xffffd93d); }

    // UI Elements
    inline juce::Colour controlBar()       { return juce::Colour (0xff0e0e1e); }
    inline juce::Colour presetBar()        { return juce::Colour (0xff161630); }
    inline juce::Colour knobBackground()   { return juce::Colour (0xff2a2a4a); }
    inline juce::Colour knobForeground()   { return juce::Colour (0xffccccdd); }
    inline juce::Colour textBright()       { return juce::Colour (0xffeeeeee); }
    inline juce::Colour textDim()          { return juce::Colour (0xff777788); }

    // Panel gradients
    inline juce::Colour panelTop()         { return juce::Colour (0xff1e1e3a); }
    inline juce::Colour panelBottom()      { return juce::Colour (0xff0a0a1a); }

    // Bevels
    inline juce::Colour bevelLight()       { return juce::Colour (0x18ffffff); }
    inline juce::Colour bevelDark()        { return juce::Colour (0x30000000); }
    inline juce::Colour screenShadow()     { return juce::Colour (0x50000000); }
    inline juce::Colour screenHighlight()  { return juce::Colour (0x08ffffff); }

    // Knob metallic
    inline juce::Colour knobEdge()         { return juce::Colour (0xff3a3a5c); }
    inline juce::Colour knobShadow()       { return juce::Colour (0x400a0a1a); }
    inline juce::Colour knobIndicator()    { return juce::Colour (0xffffd93d); }

    // Spectrum
    inline juce::Colour spectrumPre()      { return juce::Colour (0x40888899); }
    inline juce::Colour spectrumPost()     { return juce::Colour (0x606bcfff); }

    // Sidechain ring meter gradient (from green through yellow to red)
    inline juce::Colour ringMeterLow()     { return juce::Colour (0xff4caf50); }
    inline juce::Colour ringMeterMid()     { return juce::Colour (0xffffeb3b); }
    inline juce::Colour ringMeterHigh()    { return juce::Colour (0xffff5252); }
}
