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

#include <juce_audio_basics/juce_audio_basics.h>
#include <cmath>
#include <algorithm>

// Generates a sidechain trigger envelope from MIDI note on/off events.
// When a MIDI note is received, it creates a pulse with a fast attack
// and configurable release that can be used as a sidechain signal.

class MIDITrigger
{
public:
    MIDITrigger() = default;

    void prepare (double sampleRate)
    {
        sr = sampleRate;
        reset();
    }

    void reset()
    {
        envelopeLevel = 0.0f;
        noteOnVelocity = 0.0f;
    }

    // Process a block of MIDI messages and generate trigger envelope samples.
    // Writes numSamples of trigger signal into the output buffer.
    void processBlock (const juce::MidiBuffer& midiMessages, float* output, int numSamples)
    {
        // Attack: ~0.5ms for punchy trigger
        float attackCoeff = std::exp (-1.0f / (float) (0.0005 * sr));
        // Release: use the compressor's release (or a fixed ~50ms for snappy ducking)
        float releaseCoeff = std::exp (-1.0f / (float) (0.05 * sr));

        int currentSample = 0;

        for (const auto metadata : midiMessages)
        {
            auto msg = metadata.getMessage();
            int samplePos = metadata.samplePosition;

            // Generate envelope up to this MIDI event
            for (; currentSample < samplePos && currentSample < numSamples; ++currentSample)
                output[currentSample] = advanceEnvelope (attackCoeff, releaseCoeff);

            if (msg.isNoteOn())
            {
                noteOnVelocity = msg.getFloatVelocity();
                // Retrigger: immediately jump toward target
            }
            else if (msg.isNoteOff())
            {
                noteOnVelocity = 0.0f;
            }
        }

        // Fill remaining samples
        for (; currentSample < numSamples; ++currentSample)
            output[currentSample] = advanceEnvelope (attackCoeff, releaseCoeff);
    }

    float getCurrentLevel() const { return envelopeLevel; }

private:
    float advanceEnvelope (float attackCoeff, float releaseCoeff)
    {
        float target = noteOnVelocity;

        if (target > envelopeLevel)
        {
            // Attack
            envelopeLevel = attackCoeff * envelopeLevel + (1.0f - attackCoeff) * target;
        }
        else
        {
            // Release
            envelopeLevel = releaseCoeff * envelopeLevel + (1.0f - releaseCoeff) * target;
        }

        return envelopeLevel;
    }

    double sr = 48000.0;
    float envelopeLevel = 0.0f;
    float noteOnVelocity = 0.0f;
};
