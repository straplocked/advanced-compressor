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

#include <juce_core/juce_core.h>
#include <unordered_map>

namespace HelpText
{

inline juce::String getHelpText (const juce::String& componentID)
{
    static const std::unordered_map<std::string, const char*> database = {
        // ==================== Header ====================
        { "header.prevPreset",   "Previous Preset \xe2\x80\x94 cycles backward through the preset list." },
        { "header.nextPreset",   "Next Preset \xe2\x80\x94 cycles forward through the preset list." },
        { "header.presetName",   "Current Preset \xe2\x80\x94 click to browse and select factory or user presets." },
        { "header.savePreset",   "Save Preset \xe2\x80\x94 saves the current compressor settings as a user preset." },
        { "header.undo",         "Undo \xe2\x80\x94 reverses the last parameter change (Cmd+Z)." },
        { "header.redo",         "Redo \xe2\x80\x94 re-applies the last undone change (Cmd+Shift+Z)." },
        { "header.abToggle",     "A/B Compare \xe2\x80\x94 toggles between two independent compressor states for quick comparison." },
        { "header.copyAB",       "Copy A to B (or B to A) \xe2\x80\x94 copies the active state to the inactive slot." },
        { "header.delta",        "Delta Listen \xe2\x80\x94 monitors only the difference between dry and wet signal. Useful for hearing exactly what the compressor is removing." },
        { "header.midiLearn",    "MIDI Learn \xe2\x80\x94 assigns MIDI CC messages to compressor parameters for hardware control." },

        // ==================== Bottom Panel: Row 1 (Primary) ====================
        { "bottom.threshold",    "Threshold \xe2\x80\x94 the level above which compression begins (-60 to 0 dB). The green ring shows current gain reduction." },
        { "bottom.ratio",        "Ratio \xe2\x80\x94 how much the signal is reduced above threshold. 2:1 halves the excess, \xe2\x88\x9e:1 is hard limiting (1:1 to 100:1)." },
        { "bottom.attack",       "Attack \xe2\x80\x94 how quickly compression engages after the signal exceeds threshold (0.005 to 250 ms)." },
        { "bottom.release",      "Release \xe2\x80\x94 how quickly compression recovers after the signal drops below threshold (5 to 2500 ms)." },
        { "bottom.outputGain",   "Output Gain \xe2\x80\x94 adjusts overall output level after compression (-30 to +30 dB)." },
        { "bottom.style",        "Compression Style \xe2\x80\x94 selects the detection, topology, and envelope character. 14 styles from transparent Clean to colored Vari-Mu." },
        { "bottom.analyzer",     "Spectrum Analyzer \xe2\x80\x94 toggles real-time FFT spectrum display (pre/post compression)." },
        { "bottom.autoGain",     "Auto Gain \xe2\x80\x94 automatically compensates output level to match perceived loudness before compression." },

        // ==================== Bottom Panel: Row 2 (Secondary) ====================
        { "bottom.knee",         "Knee \xe2\x80\x94 how gradually compression transitions around the threshold. 0 dB = hard knee, higher = softer (0 to 72 dB)." },
        { "bottom.range",        "Range \xe2\x80\x94 limits the maximum gain reduction. 60 dB = full range, lower values cap the GR (0 to 60 dB)." },
        { "bottom.hold",         "Hold \xe2\x80\x94 time the compressor stays in gain reduction before release begins (0 to 500 ms). Useful for avoiding pumping." },
        { "bottom.lookahead",    "Lookahead \xe2\x80\x94 delays the audio so the compressor reacts before transients arrive (0 to 20 ms). Adds latency." },
        { "bottom.mix",          "Mix \xe2\x80\x94 blends dry and compressed signals for parallel (New York) compression (0 to 100%)." },
        { "bottom.wetGain",      "Wet Gain \xe2\x80\x94 adjusts the level of only the compressed signal before the dry/wet mix (-30 to +30 dB)." },
        { "bottom.stereoLink",   "Stereo Link \xe2\x80\x94 how much L/R channels share gain reduction. 100% = fully linked, 0% = dual mono (0 to 100%)." },
        { "bottom.stereoMode",   "Stereo Mode \xe2\x80\x94 Normal (L/R), Mid, Side, M>S (mid drives side GR), or S>M (side drives mid GR)." },
        { "bottom.inputGain",    "Input Gain \xe2\x80\x94 adjusts level before compression (-30 to +30 dB). Drives the signal harder into the threshold." },
        { "bottom.charDrive",    "Character Drive \xe2\x80\x94 amount of saturation applied by the character processor (0 to 24 dB)." },
        { "bottom.charMode",     "Character Mode \xe2\x80\x94 Off, Tube (warm asymmetric), Diode (hard symmetric), or Bright (odd harmonics)." },
        { "bottom.oversample",   "Oversampling \xe2\x80\x94 processes at higher sample rates for cleaner saturation. Higher = better quality but more CPU (1x to 32x)." },
        { "bottom.autoThresh",   "Auto Threshold \xe2\x80\x94 slowly tracks the input RMS level and adjusts threshold automatically for consistent compression." },
        { "bottom.meterScale",   "Meter Scale \xe2\x80\x94 sets the dB range shown on level meters: 9, 18, 36, 60, or 90 dB." },
        { "bottom.midiTrigger",  "MIDI Trigger \xe2\x80\x94 uses MIDI note-on events to trigger gain reduction instead of the audio signal." },
        { "bottom.analyzerPre",  "Analyzer Pre/Post \xe2\x80\x94 toggles whether the spectrum analyzer shows the signal before or after compression." },

        // ==================== Sidechain EQ ====================
        { "sc.toggle",           "Sidechain EQ \xe2\x80\x94 expand or collapse the 6-band sidechain EQ. Shapes the detection signal without affecting the audio output." },
        { "sc.lcOn",             "Low Cut On/Off \xe2\x80\x94 enables a high-pass filter on the sidechain to prevent bass from triggering compression." },
        { "sc.lcFreq",           "Low Cut Frequency \xe2\x80\x94 cutoff frequency for the sidechain high-pass filter (20 Hz to 20 kHz)." },
        { "sc.lcSlope",          "Low Cut Slope \xe2\x80\x94 steepness of the sidechain high-pass filter (6 to 96 dB/oct)." },
        { "sc.lsOn",             "Low Shelf On/Off \xe2\x80\x94 enables a low shelf filter on the sidechain." },
        { "sc.lsFreq",           "Low Shelf Frequency \xe2\x80\x94 corner frequency of the sidechain low shelf (20 Hz to 20 kHz)." },
        { "sc.lsGain",           "Low Shelf Gain \xe2\x80\x94 boost or cut applied to the sidechain low shelf (-30 to +30 dB)." },
        { "sc.b1Freq",           "Bell 1 Frequency \xe2\x80\x94 center frequency of the first sidechain bell filter." },
        { "sc.b1Gain",           "Bell 1 Gain \xe2\x80\x94 boost or cut for the first sidechain bell filter." },
        { "sc.b1Q",              "Bell 1 Q \xe2\x80\x94 bandwidth of the first sidechain bell filter. Higher Q = narrower band." },
        { "sc.b2Freq",           "Bell 2 Frequency \xe2\x80\x94 center frequency of the second sidechain bell filter." },
        { "sc.b2Gain",           "Bell 2 Gain \xe2\x80\x94 boost or cut for the second sidechain bell filter." },
        { "sc.b2Q",              "Bell 2 Q \xe2\x80\x94 bandwidth of the second sidechain bell filter. Higher Q = narrower band." },
        { "sc.hsOn",             "High Shelf On/Off \xe2\x80\x94 enables a high shelf filter on the sidechain." },
        { "sc.hsFreq",           "High Shelf Frequency \xe2\x80\x94 corner frequency of the sidechain high shelf." },
        { "sc.hsGain",           "High Shelf Gain \xe2\x80\x94 boost or cut applied to the sidechain high shelf." },
        { "sc.hcOn",             "High Cut On/Off \xe2\x80\x94 enables a low-pass filter on the sidechain to make compression less sensitive to highs." },
        { "sc.hcFreq",           "High Cut Frequency \xe2\x80\x94 cutoff frequency for the sidechain low-pass filter." },
        { "sc.hcSlope",          "High Cut Slope \xe2\x80\x94 steepness of the sidechain low-pass filter (6 to 96 dB/oct)." },
    };

    auto it = database.find (componentID.toStdString());
    if (it != database.end())
        return juce::String::fromUTF8 (it->second);

    return {};
}

} // namespace HelpText
