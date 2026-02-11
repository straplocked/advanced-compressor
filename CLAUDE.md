# CLAUDE.md -- Advanced Compressor

## Project Overview

Advanced Compressor is a JUCE 7 audio plugin (VST3/AU/CLAP/Standalone) implementing a professional dynamics compressor with 8 styles, sidechain EQ, M/S processing, and oversampling. Sister plugin to Advanced EQ (`../advanced-eq`).

## Build

```bash
cmake -B build && cmake --build build
```

Requires CMake 3.25+, C++20 compiler, JUCE 7.0.12 (submodule). Builds Standalone + VST3 + AU (macOS).

## Architecture

- **Lock-free audio thread**: Parameters read via `std::atomic<float>*` in `CompressorParameterAttachment`. Never allocate, lock, or do I/O on the audio thread.
- **Triple-buffered data transfer**: `TripleBuffer<T>` passes metering and FFT data from audio thread to UI thread without locks.
- **Per-sample processing**: Compression runs sample-by-sample in `CompressorEngine::process()` for accurate envelope tracking.
- **APVTS**: All 30 parameters managed via `juce::AudioProcessorValueTreeState`. Parameter layout in `ParameterLayout.cpp`.

## Key Source Paths

| Path | Purpose |
|------|---------|
| `source/PluginProcessor.h/cpp` | Audio processor, APVTS, state serialization |
| `source/PluginEditor.h/cpp` | 3-panel GUI layout |
| `source/dsp/CompressorEngine.h/cpp` | Top-level DSP chain |
| `source/dsp/CompressorCore.h/cpp` | Per-channel detector+computer+envelope |
| `source/dsp/CompressorStyle.h` | 8 styles + StyleBehavior definitions |
| `source/dsp/EnvelopeFollower.h/cpp` | Attack/release/hold with opto + program-dependent modes |
| `source/dsp/SidechainEQ.h/cpp` | 3-band SC filter using SVFFilter |
| `source/parameters/ParameterIDs.h` | All 30 parameter ID constants |
| `source/parameters/ParameterLayout.cpp` | APVTS layout factory |
| `source/parameters/CompressorParameterAttachment.h` | Lock-free atomic param bridge |
| `source/utils/Constants.h` | All ranges, defaults, UI dimensions |
| `source/ui/LookAndFeel/ColourPalette.h` | Color scheme |

## Conventions

- **Code style**: Whitespace-heavy JUCE style -- spaces inside parentheses, Allman-ish braces, 4-space indent.
- **File naming**: PascalCase for classes, matching header/source names.
- **Parameters**: String IDs like `"threshold"`, `"sc_lc_freq"`. Constants in `ParamIDs` namespace.
- **DSP classes**: `prepare(sampleRate, ...)` / `reset()` / `process(...)` pattern.
- **UI components**: JUCE `Component` subclasses with `paint()` / `resized()`. Timer-driven at 30 Hz.
- **No JUCE `Slider::Listener`**: Use APVTS attachments (`SliderAttachment`, `ComboBoxAttachment`, `ButtonAttachment`).

## Signal Flow

```
Input -> Input Gain/Pan -> [Dry copy] -> Sidechain EQ -> Level Detector
  -> Gain Computer -> Envelope Follower -> Gain Reduction
  -> Apply GR (with lookahead) -> Wet Gain -> Auto Gain
  -> Dry/Wet Mix -> Output Gain/Pan -> Output
```

Stereo modes: Normal (linked/dual-mono), Mid-only, Side-only via M/S encode/decode.

## Adding a Parameter

1. Add ID constant in `ParameterIDs.h`
2. Add range/default in `Constants.h`
3. Add to APVTS layout in `ParameterLayout.cpp`
4. Add atomic pointer + load in `CompressorParameterAttachment.h`
5. Add field to `CompressorParameters` struct
6. Use in DSP via `CompressorEngine::updateParameters()`

## Common Tasks

- **Change compression behavior**: Modify `StyleBehavior` values in `CompressorStyle.h`
- **Add a factory preset**: Add to the appropriate getter in `FactoryPresets.cpp`
- **Change UI colors**: Edit `ColourPalette.h`
- **Adjust parameter ranges**: Edit `Constants.h` and `ParameterLayout.cpp`
- **Add a new DSP stage**: Integrate in `CompressorEngine::process()`
