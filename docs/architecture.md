# Architecture

## Overview

Advanced Compressor follows the same architecture as Advanced EQ: a JUCE `AudioProcessor` with an `AudioProcessorValueTreeState` (APVTS) for parameter management, a dedicated DSP engine, and a component-based GUI. The design prioritizes lock-free audio processing and clean separation between the audio thread and the UI thread.

## System Diagram

```
┌─────────────────────────────────────────────────────┐
│  AdvancedCompressorProcessor (AudioProcessor)       │
│                                                     │
│  ┌─────────────────────┐  ┌──────────────────────┐  │
│  │  APVTS              │  │  CompressorParam-    │  │
│  │  (30 parameters)    │──│  Attachment          │  │
│  │                     │  │  (atomic<float>*)    │  │
│  └─────────────────────┘  └──────────────────────┘  │
│                                      │              │
│  ┌─────────────────────┐             │              │
│  │  PresetManager      │             │              │
│  │  (load/save/browse) │             ▼              │
│  └─────────────────────┘  ┌──────────────────────┐  │
│                           │  CompressorEngine    │  │
│  ┌─────────────────────┐  │  (top-level DSP)     │  │
│  │  FFTProcessor (pre) │  └──────────────────────┘  │
│  │  FFTProcessor (post)│             │              │
│  └─────────────────────┘             │              │
│           │                          │              │
│           ▼                          ▼              │
│  ┌─────────────────┐      ┌──────────────────────┐  │
│  │  TripleBuffer    │      │  LevelMeterData      │  │
│  │  (FFT magnitudes)│      │  (TripleBuffer)      │  │
│  └─────────────────┘      └──────────────────────┘  │
└─────────────────────────────────────────────────────┘
           │                          │
           ▼                          ▼
┌─────────────────────────────────────────────────────┐
│  AdvancedCompressorEditor (AudioProcessorEditor)    │
│                                                     │
│  ┌─────────────────────────────────────────────┐    │
│  │  HeaderToolbar (36 px)                      │    │
│  │  Presets | Style | Oversample | Analyzer    │    │
│  ├─────────────────────────────────────────────┤    │
│  │  CompressorDisplay (center, fills space)    │    │
│  │  ┌──────┬──────────┬──────────┬──────┐      │    │
│  │  │Input │  Knee    │  GR      │Output│      │    │
│  │  │Meter │  Display │  Display │Meter │      │    │
│  │  └──────┴──────────┴──────────┴──────┘      │    │
│  │  [Spectrum backdrop underneath]             │    │
│  ├─────────────────────────────────────────────┤    │
│  │  BottomControlStrip (110 px)                │    │
│  │  Knobs: Thresh|Ratio|Knee|Range|Atk|Rel|    │    │
│  │  Hold|Mix|WetGain|Link | SC Panel           │    │
│  └─────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────┘
```

## Threading Model

### Audio Thread

The audio thread runs `processBlock()` which:

1. Reads all parameters atomically via `CompressorParameterAttachment::getCurrentParameters()`
2. Passes them to `CompressorEngine::updateParameters()`
3. Runs `CompressorEngine::process()` on the audio buffer
4. Pushes audio into `FFTProcessor` (pre/post) ring buffers
5. Meter data is published via `TripleBuffer` (wait-free single-producer)

**Rules**: No allocations, no locks, no I/O, no virtual calls on unowned objects.

### UI Thread

The UI thread runs on a 30 Hz timer (`Constants::kUITimerHz`):

1. Reads meter frames from `TripleBuffer` (single-consumer)
2. Reads FFT magnitudes from `TripleBuffer`
3. Updates visual components
4. Triggers `repaint()`

### Data Transfer

| Data | Mechanism | Direction |
|------|-----------|-----------|
| Parameters | `std::atomic<float>*` | UI -> Audio |
| Meter levels | `TripleBuffer<MeterFrame>` | Audio -> UI |
| FFT magnitudes | `TripleBuffer<array<float>>` | Audio -> UI |
| Presets | APVTS `setParameterNotifyingHost()` | UI -> Audio |
| State | XML serialization (not real-time) | Host <-> Plugin |

## Module Dependency Graph

```
PluginProcessor
  ├── CompressorEngine
  │     ├── CompressorCore (x2, L/R)
  │     │     ├── LevelDetector
  │     │     ├── GainComputer
  │     │     └── EnvelopeFollower
  │     ├── SidechainEQ (x2)
  │     │     └── SVFFilter (x3: LC, Band, HC)
  │     ├── Oversampler
  │     ├── AutoGainComputer
  │     ├── MidSideProcessor
  │     └── LevelMeterData
  ├── FFTProcessor (x2, pre/post)
  ├── CompressorParameterAttachment
  └── PresetManager

PluginEditor
  ├── HeaderToolbar
  ├── CompressorDisplay
  │     ├── KneeDisplay
  │     ├── LevelMeterComponent (x2)
  │     ├── GainReductionDisplay
  │     └── SpectrumAnalyzer (x2)
  ├── BottomControlStrip
  │     ├── SidechainPanel
  │     └── SidechainMeterRing
  └── AdvancedCompressorLookAndFeel
```

## State Serialization

Plugin state is saved/restored as XML via APVTS:

```xml
<AdvancedCompressorState>
  <PARAM id="threshold" value="-20.0"/>
  <PARAM id="ratio" value="4.0"/>
  <!-- ... all 30 parameters ... -->
</AdvancedCompressorState>
```

The `PresetManager` also saves/loads user presets as XML files to the platform-appropriate Application Support directory.
