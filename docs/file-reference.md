# File Reference

Complete listing of all source files with descriptions.

## Plugin Core

| File | Description |
|------|-------------|
| `source/PluginProcessor.h/cpp` | Main `AudioProcessor` subclass. Owns APVTS, CompressorEngine, FFT processors, PresetManager. Handles `processBlock()`, state serialization, bus layout, and latency reporting. |
| `source/PluginEditor.h/cpp` | Main `AudioProcessorEditor`. 3-panel layout: HeaderToolbar (36px top), CompressorDisplay (center), BottomControlStrip (110px bottom). Default 960x620, resizable 750x450-1920x1080. |

## DSP (`source/dsp/`)

| File | Description |
|------|-------------|
| `CompressorEngine.h/cpp` | Top-level DSP chain. Orchestrates input gain/pan, sidechain routing, M/S encoding, per-sample compression with stereo linking, lookahead delay, auto gain, dry/wet mix, output gain/pan, and metering. |
| `CompressorCore.h/cpp` | Per-channel compressor combining LevelDetector, GainComputer, and EnvelopeFollower. Supports feedback topology for Classic and Bus styles. |
| `LevelDetector.h/cpp` | Audio level detection. Peak tracking with slow decay, one-pole RMS smoothing, and combined peak+RMS mode. Output in dBFS. |
| `GainComputer.h/cpp` | Static transfer characteristic. Computes gain reduction from input level using soft-knee with threshold, ratio, knee width, and range parameters. |
| `EnvelopeFollower.h/cpp` | Ballistics shaping for gain reduction. One-pole attack/release with hold, opto two-stage release (LDR model), and program-dependent release. Scales user timing values per `StyleBehavior`. |
| `CompressorStyle.h` | Defines 14 compression styles as enum + `StyleBehavior` struct. Each style specifies detection mode, topology, timing scales, knee offset, and ratio scale. |
| `SidechainEQ.h/cpp` | 6-band sidechain EQ: low cut, low shelf, 2 bell bands, high shelf, high cut with variable slopes. Uses SVFFilter. |
| `SVFFilter.h/cpp` | Cytomic Topology-Preserving Transform State Variable Filter. Copied from Advanced EQ. Supports all filter types, unconditionally stable, zipper-free. |
| `MidSideProcessor.h` | Inline M/S encode/decode. `Mid=(L+R)/2`, `Side=(L-R)/2`, perfect reconstruction. |
| `Oversampler.h/cpp` | Wrapper around `juce::dsp::Oversampling<float>`. Supports 1x/2x/4x with half-band polyphase IIR filters. Reports latency. |
| `AutoGainComputer.h/cpp` | Auto makeup gain. Tracks 500ms smoothed average of gain reduction, compensates ~50% as positive gain. |
| `FilterTypes.h` | Minimal `FilterType` enum (Bell, LowShelf, HighShelf, HighPass, LowPass, Notch, BandPass, TiltShelf, FlatTilt) used by SVFFilter. |

## Analyzer (`source/analyzer/`)

| File | Description |
|------|-------------|
| `FFTProcessor.h/cpp` | Audio-thread FFT processor. Ring buffer accumulation, Hann windowing, 4096-point FFT, magnitude-to-dB conversion. Outputs via TripleBuffer. |
| `SpectrumAnalyzer.h/cpp` | UI-thread spectrum visualization. Reads FFT magnitudes from TripleBuffer, resamples to logarithmic frequency, builds JUCE `Path` for drawing. Asymmetric smoothing (fast attack, slow decay). |
| `TripleBuffer.h` | Lock-free single-producer single-consumer triple buffer template. Wait-free on both sides. Used for FFT and meter data transfer. |

## Metering (`source/metering/`)

| File | Description |
|------|-------------|
| `LevelMeterData.h/cpp` | Bridges audio-thread level measurements to UI via TripleBuffer. `MeterFrame` struct holds input L/R, output L/R, and GR L/R in dB. |
| `PeakHoldCalculator.h` | Header-only peak hold with configurable hold time (1500ms default) and linear decay (20 dB/s default). Used by meter components. |

## Parameters (`source/parameters/`)

| File | Description |
|------|-------------|
| `ParameterIDs.h` | 51 string ID constants (`ParamIDs::kThreshold`, etc.) plus helper functions returning `StringArray` for style/mode/oversampling names. |
| `ParameterLayout.h/cpp` | APVTS layout factory. Creates all 51 parameters with ranges, defaults, and skew factors. |
| `CompressorParameterAttachment.h` | Lock-free parameter bridge. Caches 51 `std::atomic<float>*` pointers. `getCurrentParameters()` performs atomic loads into a `CompressorParameters` struct. |

## Presets (`source/presets/`)

| File | Description |
|------|-------------|
| `PresetData.h` | `CompressorPresetData` struct (threshold, ratio, knee, range, attack, release, hold, style, mix, outputGain) and `Preset` struct (name, category, data, factory flag, source file). |
| `PresetManager.h/cpp` | Preset management: browse, apply, save, delete. Loads factory presets and scans user presets from disk. Listener interface for change notifications. |
| `FactoryPresets.h/cpp` | 19 factory presets across 5 categories (Vocal, Drums, Bus, Mastering, Creative). |

## UI Components (`source/ui/components/`)

| File | Description |
|------|-------------|
| `HeaderToolbar.h/cpp` | Top toolbar: plugin title, preset navigation (prev/next/name/save), style selector, oversample selector, analyzer toggle, output gain knob. Implements PresetManager::Listener. |
| `CompressorDisplay.h/cpp` | Main display area. Orchestrates KneeDisplay, input/output LevelMeterComponents, GainReductionDisplay, and two SpectrumAnalyzers (pre/post). Timer at 30 Hz polls meter data. |
| `KneeDisplay.h/cpp` | Transfer curve widget. Draws grid, 1:1 line, threshold marker, knee region shading, computed transfer curve, and animated operating point dot. |
| `LevelMeterComponent.h/cpp` | Vertical stereo level meter with green-yellow-red gradient bars and peak hold indicators. |
| `GainReductionDisplay.h/cpp` | Top-down red bar showing gain reduction with dB text readout. |
| `SidechainMeterRing.h/cpp` | Circular arc meter around the threshold knob showing GR amount in a green-yellow-red gradient. |
| `SidechainPanel.h/cpp` | Panel with 4 rotary knobs (LC freq, band freq, band gain, HC freq) and 2 toggle buttons (LC on, HC on) for sidechain EQ control. |
| `BottomControlStrip.h/cpp` | Bottom panel with 10 rotary knobs (threshold, ratio, knee, range, attack, release, hold, mix, wet gain, stereo link), stereo mode combo, embedded SidechainPanel and SidechainMeterRing. |

## UI Styling (`source/ui/LookAndFeel/`)

| File | Description |
|------|-------------|
| `ColourPalette.h` | Centralized color scheme as inline functions. Dark blue-gray theme with red GR, yellow threshold/curve, green-yellow-red meters. |
| `AdvancedCompressorLookAndFeel.h` | Custom `LookAndFeel_V4` with metallic rotary knobs (radial gradient, value arc, notch marks), gradient buttons with bevel, and styled combo boxes. |

## Utilities (`source/utils/`)

| File | Description |
|------|-------------|
| `Constants.h` | All compile-time constants: parameter ranges/defaults, frequency limits, FFT config, meter ranges, UI dimensions. |
| `MathUtils.h` | `dbToGain()`, `gainToDb()`, `frequencyToNormalized()`, `normalizedToFrequency()`, `frequencyToX()`, `xToFrequency()`, `gainToY()`, `yToGain()`. |
| `FrequencyUtils.h` | `midiNoteToFrequency()`, `frequencyToMidiNote()`, `midiNoteToName()`, `frequencyToNoteName()`, `formatFrequency()`. |
| `CompressorMath.h` | `timeToCoeff()` (ms to one-pole coefficient), `msToSamples()`, `computeGainReduction()` (soft-knee static characteristic). |

## Build System

| File | Description |
|------|-------------|
| `CMakeLists.txt` | Main build config. Target `AdvancedCompressor`, codes ACmP/ACm1, VST3+AU+Standalone+CLAP+AAX formats, C++20, -ffast-math. |
| `cmake/PlatformDefaults.cmake` | Platform compiler flags: macOS universal binary (arm64+x86_64), deployment target 10.13, warning levels. |
| `.gitmodules` | Git submodules: JUCE 7.0.12 + clap-juce-extensions. |
