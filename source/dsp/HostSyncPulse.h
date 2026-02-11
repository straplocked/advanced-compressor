#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <cmath>

// Generates tempo-synchronized sidechain pulses using host transport info.
// Reads BPM and position from AudioPlayHead and outputs a shaped pulse
// at configurable note divisions.

class HostSyncPulse
{
public:
    HostSyncPulse() = default;

    void prepare (double sampleRate)
    {
        sr = sampleRate;
        reset();
    }

    void reset()
    {
        phase = 0.0;
        lastPpqPosition = -1.0;
    }

    // Division indices match getSyncDivisionNames():
    // 0=1/1, 1=1/2, 2=1/4, 3=1/8, 4=1/16, 5=1/32,
    // 6=1/4 D, 7=1/8 D, 8=1/4 T, 9=1/8 T
    void setDivision (int divIndex) { division = divIndex; }

    // Shape indices match getSyncShapeNames():
    // 0=Square, 1=Triangle, 2=Sine, 3=Sawtooth
    void setShape (int shapeIndex) { shape = shapeIndex; }

    // Generate a block of sync pulse samples using host transport info.
    // playHead may be null (produces silence).
    void processBlock (juce::AudioPlayHead* playHead, float* output, int numSamples)
    {
        double bpm = 120.0;
        double ppqPosition = 0.0;
        bool isPlaying = false;

        if (playHead != nullptr)
        {
            auto posInfo = playHead->getPosition();
            if (posInfo.hasValue())
            {
                if (posInfo->getBpm().hasValue())
                    bpm = *posInfo->getBpm();
                if (posInfo->getPpqPosition().hasValue())
                    ppqPosition = *posInfo->getPpqPosition();
                isPlaying = posInfo->getIsPlaying();
            }
        }

        if (!isPlaying || bpm <= 0.0)
        {
            std::fill (output, output + numSamples, 0.0f);
            phase = 0.0;
            return;
        }

        // Convert division to beats
        double beatsPerPulse = divisionToBeats();

        // Frequency of the pulse in Hz
        double pulseFreqHz = (bpm / 60.0) / beatsPerPulse;

        // Phase increment per sample
        double phaseInc = pulseFreqHz / sr;

        // Sync phase to host position
        if (lastPpqPosition < 0.0)
        {
            // First call: align phase to ppq
            phase = std::fmod (ppqPosition / beatsPerPulse, 1.0);
        }
        else
        {
            // Subsequent calls: drift correction
            double expectedPhase = std::fmod (ppqPosition / beatsPerPulse, 1.0);
            double phaseDiff = expectedPhase - phase;
            if (phaseDiff > 0.5) phaseDiff -= 1.0;
            if (phaseDiff < -0.5) phaseDiff += 1.0;
            // Gentle correction over the block
            phase += phaseDiff * 0.1;
        }

        lastPpqPosition = ppqPosition;

        for (int i = 0; i < numSamples; ++i)
        {
            output[i] = computeShape (phase);
            phase += phaseInc;
            if (phase >= 1.0)
                phase -= 1.0;
        }
    }

private:
    double divisionToBeats() const
    {
        switch (division)
        {
            case 0:  return 4.0;     // 1/1 (whole note = 4 beats)
            case 1:  return 2.0;     // 1/2
            case 2:  return 1.0;     // 1/4
            case 3:  return 0.5;     // 1/8
            case 4:  return 0.25;    // 1/16
            case 5:  return 0.125;   // 1/32
            case 6:  return 1.5;     // 1/4 dotted
            case 7:  return 0.75;    // 1/8 dotted
            case 8:  return 2.0 / 3.0;  // 1/4 triplet
            case 9:  return 1.0 / 3.0;  // 1/8 triplet
            default: return 1.0;
        }
    }

    float computeShape (double ph) const
    {
        // ph is 0..1 representing one pulse cycle
        // Output is 0..1 where 1 = full trigger
        switch (shape)
        {
            case 0:  // Square: first 50% is high, rest is low
                return ph < 0.5 ? 1.0f : 0.0f;

            case 1:  // Triangle: ramp up then down
            {
                if (ph < 0.5)
                    return (float) (1.0 - ph * 2.0);
                else
                    return (float) ((ph - 0.5) * 2.0);
            }

            case 2:  // Sine: half-sine pulse (positive half)
            {
                float val = (float) std::sin (ph * 3.14159265358979323846);
                return val * val;  // raised sine for smoother pulse
            }

            case 3:  // Sawtooth: starts high, ramps down
                return (float) (1.0 - ph);

            default:
                return 0.0f;
        }
    }

    double sr = 48000.0;
    double phase = 0.0;
    double lastPpqPosition = -1.0;
    int division = 2;   // default 1/4
    int shape = 0;      // default Square
};
