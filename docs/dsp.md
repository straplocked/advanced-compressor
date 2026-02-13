# DSP Engine

## Signal Flow

```
Input Buffer (stereo)
  │
  ├── Input Gain (dB to linear)
  ├── Input Pan (constant-power)
  │
  ├── Measure input levels → LevelMeterData
  ├── Save dry copy for mix
  │
  ├── M/S Encode (if Mid or Side mode)
  │
  │   For each sample:
  │   ├── Select sidechain source (internal or external)
  │   ├── SidechainEQ (LC + Band + HC)
  │   ├── CompressorCore L: detect → compute GR → envelope
  │   ├── CompressorCore R: detect → compute GR → envelope
  │   ├── Stereo Link: interpolate between per-channel and linked GR
  │   ├── M/S mode: zero out the non-target channel's GR
  │   ├── Lookahead delay (if enabled)
  │   └── Apply gain reduction
  │
  ├── M/S Decode (if Mid or Side mode)
  │
  ├── Push GR to meter data
  │
  ├── Wet Gain
  ├── Auto Gain (if enabled)
  ├── Dry/Wet Mix (0-200%)
  │
  ├── SC Audition (replace output with SC signal if enabled)
  │
  ├── Output Gain
  ├── Output Pan
  │
  ├── Measure output levels → LevelMeterData
  │
  └── Output Buffer
```

## CompressorCore

Each stereo channel has its own `CompressorCore` instance containing three stages:

### 1. LevelDetector

Converts audio samples to dB level using peak, RMS, or combined detection.

- **Peak**: Tracks absolute sample values with slow decay (0.9999 coefficient per sample)
- **RMS**: One-pole IIR smoothing of squared samples, configurable window via `CompressorMath::timeToCoeff()`
- **Combined**: Returns the maximum of peak and RMS values (used by Vocal style)

Output: level in dBFS.

### 2. GainComputer

Computes static gain reduction from the input level using the transfer characteristic:

```
If inputDB <= threshold - halfKnee:
    GR = 0 dB  (below knee, no compression)

If inputDB >= threshold + halfKnee:
    GR = excess * (1 - 1/ratio)  (above knee, full compression)

If in knee region:
    GR = (1 - 1/ratio) * x^2 / (2 * kneeWidth)  (quadratic transition)

GR = min(GR, range)  (range limiting)
```

Parameters: threshold (-60 to 0 dB), ratio (1:1 to 100:1), knee (0-72 dB), range (0-60 dB).

### 3. EnvelopeFollower

Smooths the instantaneous gain reduction with style-dependent ballistics:

- **Standard mode**: One-pole attack/release with hold counter
- **Opto mode**: Two-stage release where the blend between fast and slow release depends on GR depth (models LDR behavior)
- **Program-dependent release**: Release speed adapts to GR depth (deeper compression = faster release)

The `StyleBehavior` struct scales the user's attack/release/hold values per style.

### Feedback Topology

Classic and Bus styles use feedback topology: the previous sample's gain reduction modifies the detector input, creating the self-regulating character of vintage compressors.

```
scInput = sample * dbToGain(feedbackGR)
level = detector.processSample(scInput)
gr = gainComputer.computeGainReduction(level)
smoothedGR = envelope.processSample(gr)
feedbackGR = smoothedGR  // stored for next sample
```

## Compression Styles

Each style is defined by a `StyleBehavior` struct in `CompressorStyle.h`:

| Style | Peak | RMS | Feedback | Attack Scale | Release Scale | Knee Offset | Ratio Scale | Special |
|-------|------|-----|----------|-------------|--------------|-------------|-------------|---------|
| Clean | Yes | No | No | 1.0x | 1.0x | +0 dB | 1.0x | Prog.dep. release |
| Classic | No | Yes | Yes | 1.2x | 1.5x | +0 dB | 1.0x | Prog.dep. release |
| Opto | No | Yes | No | 2.0x | 3.0x | +3 dB | 0.8x | Two-stage release |
| Vocal | Yes | Yes | No | 0.8x | 1.0x | +4 dB | 0.9x | Prog.dep. release |
| Mastering | No | Yes | No | 1.5x | 1.2x | +6 dB | 1.0x | -- |
| Bus | No | Yes | Yes | 1.0x | 1.0x | +3 dB | 1.0x | Prog.dep. release |
| Punch | Yes | No | No | 0.3x | 0.5x | +0 dB | 1.0x | -- |
| Pumping | Yes | No | No | 0.5x | 2.5x | +0 dB | 1.2x | -- |
| Versatile | Yes | Yes | No | 1.0x | 1.0x | +2 dB | 1.0x | Prog.dep. release |
| Smooth | No | Yes | No | 2.5x | 2.0x | +12 dB | 0.7x | -- |
| Op-El | No | Yes | No | 1.5x | 2.5x | +4 dB | 0.85x | Gain-dep. attack |
| Vari-Mu | No | Yes | Yes | 1.8x | 1.5x | +8 dB | 1.0x | Variable ratio |
| Upward | No | Yes | No | 1.0x | 1.2x | +4 dB | 1.0x | Upward compression |
| TTM | No | Yes | No | 1.0x | 1.0x | +4 dB | 1.0x | Down + upward combined |

## Sidechain EQ

Six SVF filters in series on the sidechain signal:

1. **Low Cut** (High-Pass): 20 Hz - 20 kHz, toggleable, variable slope (6/12/24/48/96 dB/oct)
2. **Low Shelf**: 20 Hz - 20 kHz, gain -15 to +15 dB, toggleable
3. **Band 1** (Bell): 20 Hz - 20 kHz, gain -15 to +15 dB, Q 0.1-10
4. **Band 2** (Bell): 20 Hz - 20 kHz, gain -15 to +15 dB, Q 0.1-10
5. **High Shelf**: 20 Hz - 20 kHz, gain -15 to +15 dB, toggleable
6. **High Cut** (Low-Pass): 20 Hz - 20 kHz, toggleable, variable slope (6/12/24/48/96 dB/oct)

Uses the Cytomic TPT SVF filter implementation copied from Advanced EQ, which is unconditionally stable and zipper-free under modulation.

## Stereo Processing

### Stereo Link

Continuously variable from 0% (dual-mono) to 100% (fully linked):

```
linkedGR = min(grL, grR)         // take the greater reduction
grL = grL + link * (linkedGR - grL)
grR = grR + link * (linkedGR - grR)
```

### Mid/Side Modes

- **Normal**: Standard stereo processing
- **Mid**: M/S encode -> compress mid channel only (grR = 0) -> M/S decode
- **Side**: M/S encode -> compress side channel only (grL = 0) -> M/S decode
- **M>S**: Mid channel sidechain drives side channel compression
- **S>M**: Side channel sidechain drives mid channel compression

M/S encoding: `Mid = (L+R)/2`, `Side = (L-R)/2`
M/S decoding: `L = Mid + Side`, `R = Mid - Side`

## Lookahead

Circular delay line up to 20 ms. The sidechain detector sees the signal before the delay, allowing gain reduction to be applied before transients arrive. Reported as plugin latency for DAW compensation.

## Auto Gain

Tracks a smoothed average of the gain reduction (500 ms one-pole) and compensates approximately 50% of the average GR as makeup gain:

```
autoGainDB = -smoothedGR * 0.5
```

## Dry/Wet Mix

Standard mix formula with 0-200% range for parallel compression effects:

```
output = dry + mixAmount * (wet - dry)
```

At 100% = fully wet (standard). At 50% = NY-style parallel compression. At 200% = over-compressed effect.

## Oversampling

Wraps `juce::dsp::Oversampling<float>` with half-band polyphase IIR filters:

| Setting | Factor | Latency |
|---------|--------|---------|
| 1x | 1 | 0 samples |
| 2x | 2 | ~8 samples |
| 4x | 4 | ~24 samples |
| 8x | 8 | ~56 samples |
| 16x | 16 | ~120 samples |
| 32x | 32 | ~248 samples |

Latency is reported to the host alongside lookahead latency.

## Key Math Functions

All in `CompressorMath.h`:

- `timeToCoeff(timeMs, sampleRate)`: Converts attack/release time to one-pole coefficient: `exp(-1 / (time_s * sr))`
- `msToSamples(ms, sampleRate)`: Converts milliseconds to integer sample count
- `computeGainReduction(inputDB, threshold, ratio, knee, range)`: Full soft-knee gain computation
