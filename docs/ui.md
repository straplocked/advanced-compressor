# User Interface

## Layout

The editor uses a 3-panel layout at a default size of 960x620 pixels, resizable from 750x450 to 1920x1080.

```
┌──────────────────────────────────────────────────────┐
│  HeaderToolbar (36 px)                               │
│  [<][>] [Preset Name] [Save] | [Style] [OS] [Ana]   │
├──────────────────────────────────────────────────────┤
│                                                      │
│  CompressorDisplay (fills remaining height)           │
│  ┌──────┬──────────────┬──────────────┬──────┐       │
│  │Input │              │              │Output│       │
│  │Meter │  KneeDisplay │  GR Display  │Meter │       │
│  │ L|R  │  (transfer   │  (animated   │ L|R  │       │
│  │      │   curve +    │   bar +      │      │       │
│  │      │   operating  │   dB text)   │      │       │
│  │      │   point)     │              │      │       │
│  └──────┴──────────────┴──────────────┴──────┘       │
│         [Spectrum analyzer backdrop]                 │
│                                                      │
├──────────────────────────────────────────────────────┤
│  BottomControlStrip (110 px)                         │
│  [Threshold] [Ratio] [Knee] [Range] [Attack]         │
│  [Release] [Hold] [Mix] [Wet] [Link] [Mode]         │
│  [SC: LC Freq | Band Freq/Gain | HC Freq]            │
└──────────────────────────────────────────────────────┘
```

## Components

### HeaderToolbar

Top bar with plugin title and global controls.

**Source**: `ui/components/HeaderToolbar.h/cpp`

| Element | Type | Function |
|---------|------|----------|
| Prev/Next buttons | TextButton | Navigate presets |
| Preset name | TextButton | Opens preset menu popup |
| Save button | TextButton | Opens save dialog |
| Style selector | ComboBox | 14 compression styles |
| Oversample selector | ComboBox | 1x/2x/4x/8x/16x/32x |
| Analyzer toggle | TextButton | Show/hide spectrum |
| Output gain knob | Rotary slider | Output level control |

Implements `PresetManager::Listener` for preset change notifications.

### CompressorDisplay

Main visualization area that orchestrates several sub-components.

**Source**: `ui/components/CompressorDisplay.h/cpp`

Runs a timer at 30 Hz to poll meter data from the processor's `TripleBuffer` and update all sub-components.

**Sub-components**:

#### KneeDisplay

**Source**: `ui/components/KneeDisplay.h/cpp`

Draws the compressor's transfer characteristic curve (input dB vs output dB):

- Grid lines with dB labels
- 1:1 unity reference line
- Threshold vertical marker
- Knee region shading
- Transfer curve computed via `CompressorMath::computeGainReduction()`
- Animated operating point dot showing current input level

#### LevelMeterComponent

**Source**: `ui/components/LevelMeterComponent.h/cpp`

Vertical stereo level meters with:

- Green-yellow-red gradient bars
- Peak hold indicators with configurable decay
- Range: -60 to +6 dBFS

Used for both input and output metering.

#### GainReductionDisplay

**Source**: `ui/components/GainReductionDisplay.h/cpp`

Shows gain reduction as a top-down red bar with dB text readout. Range: 0 to -40 dB.

#### SpectrumAnalyzer

**Source**: `analyzer/SpectrumAnalyzer.h/cpp`

FFT-based spectrum visualization displayed as a backdrop behind the other components. Two instances: pre-compression (gray) and post-compression (blue). 4096-point FFT with Hann windowing, logarithmic frequency mapping, and asymmetric smoothing (fast attack, slow decay).

### BottomControlStrip

Bottom panel with all compressor parameter knobs.

**Source**: `ui/components/BottomControlStrip.h/cpp`

| Knob | Parameter |
|------|-----------|
| Threshold | Threshold (-60 to 0 dB) |
| Ratio | Ratio (1:1 to 100:1) |
| Knee | Knee width (0-72 dB) |
| Range | Range limit (0-60 dB) |
| Attack | Attack time (0.005-250 ms) |
| Release | Release time (5-2500 ms) |
| Hold | Hold time (0-500 ms) |
| Mix | Dry/wet mix (0-200%) |
| Wet Gain | Wet signal gain (-30 to 30 dB) |
| Stereo Link | Link amount (0-100%) |
| Stereo Mode | Normal/Mid/Side/M>S/S>M (combo) |

Includes a `SidechainMeterRing` overlaid on the Threshold knob and an embedded `SidechainPanel`.

#### SidechainPanel

**Source**: `ui/components/SidechainPanel.h/cpp`

Panel with 6-band sidechain EQ controls:

- Low Cut: frequency knob, on/off toggle, slope selector (6-96 dB/oct)
- Low Shelf: frequency knob, gain knob, on/off toggle
- Band 1: frequency knob, gain knob, Q knob
- Band 2: frequency knob, gain knob, Q knob
- High Shelf: frequency knob, gain knob, on/off toggle
- High Cut: frequency knob, on/off toggle, slope selector (6-96 dB/oct)

All attached to APVTS parameters.

#### SidechainMeterRing

**Source**: `ui/components/SidechainMeterRing.h/cpp`

Circular arc meter drawn around the threshold knob showing current gain reduction amount. Drawn backward from the arc end in red (green-yellow-red gradient).

## Look & Feel

### ColourPalette

**Source**: `ui/LookAndFeel/ColourPalette.h`

Centralized color scheme as inline functions:

| Category | Key Colors |
|----------|-----------|
| Background | Dark blue-gray (#121228) |
| Gain Reduction | Red (#ff4444) |
| Threshold | Yellow (#ffd93d), Orange (#ffa726) |
| Meters | Green (#4caf50), Yellow (#ffeb3b), Red (#ff5252) |
| Transfer Curve | Yellow (#ffd93d) |
| Spectrum Pre | Gray (40% opacity) |
| Spectrum Post | Light blue (60% opacity) |
| Knobs | Metallic gray with yellow indicator |

### AdvancedCompressorLookAndFeel

**Source**: `ui/LookAndFeel/AdvancedCompressorLookAndFeel.h`

Custom `LookAndFeel_V4` subclass with overrides for:

- **Rotary sliders**: Metallic knob with radial gradient, drop shadow, value arc, pointer dot, and notch marks
- **Buttons**: Gradient fill with bevel highlights and toggle state indication
- **Combo boxes**: Gradient fill with dropdown arrow

## Timer-Driven Updates

The UI does not use listeners for real-time meter updates. Instead:

1. Audio thread pushes data into `TripleBuffer` (wait-free)
2. `CompressorDisplay::timerCallback()` runs at 30 Hz
3. Reads latest `MeterFrame` from triple buffer
4. Updates meter components and repaints

This ensures zero contention between audio and UI threads.
