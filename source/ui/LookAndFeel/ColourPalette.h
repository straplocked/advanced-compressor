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
    inline juce::Colour background()       { return juce::Colour (0xff040408); }
    inline juce::Colour curveArea()         { return juce::Colour (0xff0e0e18); }
    inline juce::Colour gridLine()          { return juce::Colour (0xff141420); }
    inline juce::Colour gridLineStrong()    { return juce::Colour (0xff1e1e2c); }
    inline juce::Colour axisText()          { return juce::Colour (0xff6a7a6a); }

    // Compressor-specific colors
    inline juce::Colour gainReduction()     { return juce::Colour (0xffff4444); }
    inline juce::Colour gainReductionFill() { return juce::Colour (0x40ff4444); }
    inline juce::Colour thresholdLine()     { return juce::Colour (0xff7aff3a); }
    inline juce::Colour thresholdAccent()   { return juce::Colour (0xff66cc33); }
    inline juce::Colour meterGreen()        { return juce::Colour (0xff4caf50); }
    inline juce::Colour meterYellow()       { return juce::Colour (0xffffeb3b); }
    inline juce::Colour meterRed()          { return juce::Colour (0xffff5252); }
    inline juce::Colour kneeRegion()        { return juce::Colour (0x207aff3a); }
    inline juce::Colour operatingPoint()    { return juce::Colour (0xffffffff); }
    inline juce::Colour transferCurve()     { return juce::Colour (0xff7aff3a); }

    // UI Elements
    inline juce::Colour controlBar()       { return juce::Colour (0xff040408); }
    inline juce::Colour presetBar()        { return juce::Colour (0xff080810); }
    inline juce::Colour knobBackground()   { return juce::Colour (0xff141422); }
    inline juce::Colour knobForeground()   { return juce::Colour (0xffbbbbcc); }
    inline juce::Colour textBright()       { return juce::Colour (0xffe8e8e8); }
    inline juce::Colour textDim()          { return juce::Colour (0xff5a6a5a); }

    // Panel gradients
    inline juce::Colour panelTop()         { return juce::Colour (0xff0a0a14); }
    inline juce::Colour panelBottom()      { return juce::Colour (0xff030306); }

    // Bevels
    inline juce::Colour bevelLight()       { return juce::Colour (0x14ffffff); }
    inline juce::Colour bevelDark()        { return juce::Colour (0x38000000); }
    inline juce::Colour screenShadow()     { return juce::Colour (0x58000000); }
    inline juce::Colour screenHighlight()  { return juce::Colour (0x06ffffff); }

    // Knob metallic
    inline juce::Colour knobEdge()         { return juce::Colour (0xff1e1e34); }
    inline juce::Colour knobShadow()       { return juce::Colour (0x40020204); }
    inline juce::Colour knobIndicator()    { return juce::Colour (0xff7aff3a); }

    // Spectrum
    inline juce::Colour spectrumPre()      { return juce::Colour (0x30667766); }
    inline juce::Colour spectrumPost()     { return juce::Colour (0x507aff3a); }

    // Sidechain ring meter gradient (from green through yellow to red)
    inline juce::Colour ringMeterLow()     { return juce::Colour (0xff4caf50); }
    inline juce::Colour ringMeterMid()     { return juce::Colour (0xffffeb3b); }
    inline juce::Colour ringMeterHigh()    { return juce::Colour (0xffff5252); }
}
