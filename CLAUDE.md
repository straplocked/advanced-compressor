# CLAUDE.md -- Advanced Compressor

## Project Overview

Advanced Compressor is a JUCE 7 audio plugin (VST3/AU/CLAP/Standalone) implementing a professional dynamics compressor with 12 styles, character saturation, sidechain EQ, M/S processing, oversampling up to 32x, undo/redo, A/B comparison, and delta monitoring. Sister plugin to Advanced EQ (`../advanced-eq`).

## Build

```bash
cmake -B build && cmake --build build
```

Requires CMake 3.25+, C++20 compiler, JUCE 7.0.12 (submodule). Builds Standalone + VST3 + AU (macOS).

## Architecture

- **Lock-free audio thread**: Parameters read via `std::atomic<float>*` in `CompressorParameterAttachment`. Never allocate, lock, or do I/O on the audio thread.
- **Triple-buffered data transfer**: `TripleBuffer<T>` passes metering and FFT data from audio thread to UI thread without locks.
- **Per-sample processing**: Compression runs sample-by-sample in `CompressorEngine::process()` for accurate envelope tracking.
- **APVTS with Undo**: All 37 parameters managed via `juce::AudioProcessorValueTreeState` backed by `juce::UndoManager`. Parameter layout in `ParameterLayout.cpp`.
- **A/B comparison**: Two `ValueTree` snapshots swapped via `apvts.replaceState()`.

## Key Source Paths

| Path | Purpose |
|------|---------|
| `source/PluginProcessor.h/cpp` | Audio processor, APVTS, undo manager, A/B state, serialization |
| `source/PluginEditor.h/cpp` | 3-panel GUI layout |
| `source/dsp/CompressorEngine.h/cpp` | Top-level DSP chain (compression + character + auto threshold) |
| `source/dsp/CompressorCore.h/cpp` | Per-channel detector+computer+envelope |
| `source/dsp/CompressorStyle.h` | 12 styles + StyleBehavior definitions |
| `source/dsp/EnvelopeFollower.h/cpp` | Attack/release/hold with opto, program-dependent, and gain-dependent modes |
| `source/dsp/GainComputer.h` | Soft-knee gain computation with optional variable ratio (Vari-Mu) |
| `source/dsp/CharacterProcessor.h` | Tube/Diode/Bright saturation waveshapers with drive |
| `source/dsp/AutoThreshold.h` | Slow RMS follower for level-independent threshold tracking |
| `source/dsp/SidechainEQ.h/cpp` | 3-band SC filter using SVFFilter |
| `source/parameters/ParameterIDs.h` | All 37 parameter ID constants + name lists |
| `source/parameters/ParameterLayout.cpp` | APVTS layout factory |
| `source/parameters/CompressorParameterAttachment.h` | Lock-free atomic param bridge |
| `source/utils/Constants.h` | All ranges, defaults, UI dimensions |
| `source/ui/LookAndFeel/ColourPalette.h` | Color scheme |

## Compression Styles (12)

| Style | Detection | Topology | Character |
|-------|-----------|----------|-----------|
| Clean | Peak | Feedforward | Transparent, program-dependent release |
| Classic | RMS | Feedback | Vintage glue, program-dependent release |
| Opto | RMS | Feedforward | Two-stage release (LDR model) |
| Vocal | Peak+RMS | Feedforward | Adaptive knee/ratio |
| Mastering | RMS | Feedforward | Maximum transparency |
| Bus | RMS | Feedback | Cohesive glue |
| Punch | Peak | Feedforward | Fast, transient-preserving |
| Pumping | Peak | Feedforward | Exaggerated envelope |
| Versatile | Peak+RMS | Feedforward | Balanced general-purpose |
| Smooth | RMS | Feedforward | Super-smooth, large knee |
| Op-El | RMS | Feedforward | Gain-dependent attack (warm opto-tube) |
| Vari-Mu | RMS | Feedback | Variable ratio with signal level |

## Character Modes

- **Off**: No saturation
- **Tube**: Asymmetric tanh waveshaper (warm, soft clipping)
- **Diode**: Symmetric hard clipping with soft knee
- **Bright**: Chebyshev polynomial adding odd harmonics

Each mode has an adjustable **Drive** parameter (0-24 dB) with automatic gain compensation and DC blocking.

## Signal Flow

```
Input -> Input Gain/Pan -> [Measure input + feed Auto Threshold]
  -> [Dry copy] -> Sidechain EQ -> Level Detector
  -> Gain Computer (with optional variable ratio)
  -> Envelope Follower (style-dependent: opto/program-dep/gain-dep)
  -> Gain Reduction -> Apply GR (with lookahead)
  -> M/S decode -> Character Processing (Tube/Diode/Bright + Drive)
  -> Wet Gain -> Auto Gain -> Delta (wet-dry, optional)
  -> Dry/Wet Mix -> SC Audition (optional)
  -> Output Gain/Pan -> Output
```

Stereo modes: Normal, Mid-only, Side-only, M>S (mid drives side), S>M (side drives mid).

## Conventions

- **Code style**: Whitespace-heavy JUCE style -- spaces inside parentheses, Allman-ish braces, 4-space indent.
- **File naming**: PascalCase for classes, matching header/source names.
- **Parameters**: String IDs like `"threshold"`, `"sc_lc_freq"`, `"char_mode"`. Constants in `ParamIDs` namespace.
- **DSP classes**: `prepare(sampleRate, ...)` / `reset()` / `process(...)` pattern. Header-only for simple processors (CharacterProcessor, AutoThreshold).
- **UI components**: JUCE `Component` subclasses with `paint()` / `resized()`. Timer-driven at 30 Hz.
- **No JUCE `Slider::Listener`**: Use APVTS attachments (`SliderAttachment`, `ComboBoxAttachment`, `ButtonAttachment`).

## Adding a Parameter

1. Add ID constant in `ParameterIDs.h`
2. Add range/default in `Constants.h`
3. Add to APVTS layout in `ParameterLayout.cpp`
4. Add atomic pointer + load in `CompressorParameterAttachment.h`
5. Add field to `CompressorParameters` struct
6. Use in DSP via `CompressorEngine::updateParameters()`

## Adding a Compression Style

1. Add enum value in `CompressorStyle.h`
2. Add `StyleBehavior` case in `getStyleBehavior()`
3. Add name to `getStyleNames()` in `ParameterIDs.h`
4. Update `Constants::kNumStyles`
5. If special behavior needed, update `EnvelopeFollower` and/or `GainComputer`

## Common Tasks

- **Change compression behavior**: Modify `StyleBehavior` values in `CompressorStyle.h`
- **Add a factory preset**: Add to the appropriate getter in `FactoryPresets.cpp`
- **Change UI colors**: Edit `ColourPalette.h`
- **Adjust parameter ranges**: Edit `Constants.h` and `ParameterLayout.cpp`
- **Add a new DSP stage**: Integrate in `CompressorEngine::process()`
- **Add a character mode**: Add waveshaper function in `CharacterProcessor.h`, add enum value
