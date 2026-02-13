# Parameters

Advanced Compressor has 51 automatable parameters organized into 9 groups. All parameters are managed via JUCE's `AudioProcessorValueTreeState` (APVTS).

## Parameter Reference

### Dynamics (8 parameters)

| ID | Name | Type | Range | Default | Skew | Unit |
|----|------|------|-------|---------|------|------|
| `threshold` | Threshold | Float | -60 to 0 | -20.0 | 1.0 | dB |
| `ratio` | Ratio | Float | 1.0 to 100.0 | 4.0 | 0.3 | :1 |
| `knee` | Knee | Float | 0 to 72 | 6.0 | 0.5 | dB |
| `range` | Range | Float | 0 to 60 | 60.0 | 0.5 | dB |
| `attack` | Attack | Float | 0.005 to 250 | 10.0 | 0.3 | ms |
| `release` | Release | Float | 5 to 2500 | 100.0 | 0.3 | ms |
| `hold` | Hold | Float | 0 to 500 | 0.0 | 0.4 | ms |
| `lookahead` | Lookahead | Float | 0 to 20 | 0.0 | 0.5 | ms |

### Style (1 parameter)

| ID | Name | Type | Values | Default |
|----|------|------|--------|---------|
| `comp_style` | Style | Choice | Clean, Classic, Opto, Vocal, Mastering, Bus, Punch, Pumping, Versatile, Smooth, Op-El, Vari-Mu, Upward, TTM | Clean |

### Character (2 parameters)

| ID | Name | Type | Range/Values | Default |
|----|------|------|--------------|---------|
| `char_mode` | Character | Choice | Off, Tube, Diode, Bright | Off |
| `char_drive` | Drive | Float | 0 to 24 | 0.0 dB |

### Auto (1 parameter)

| ID | Name | Type | Default |
|----|------|------|---------|
| `auto_thresh` | Auto Threshold | Bool | false |

### Gain / Mix (7 parameters)

| ID | Name | Type | Range | Default | Unit |
|----|------|------|-------|---------|------|
| `input_gain` | Input Gain | Float | -30 to 30 | 0.0 | dB |
| `input_pan` | Input Pan | Float | -1.0 to 1.0 | 0.0 | |
| `output_gain` | Output Gain | Float | -30 to 30 | 0.0 | dB |
| `output_pan` | Output Pan | Float | -1.0 to 1.0 | 0.0 | |
| `wet_gain` | Wet Gain | Float | -30 to 30 | 0.0 | dB |
| `auto_gain` | Auto Gain | Bool | | false | |
| `mix` | Mix | Float | 0 to 200 | 100.0 | % |

### Sidechain (21 parameters)

| ID | Name | Type | Range | Default | Unit |
|----|------|------|-------|---------|------|
| `sc_external` | SC External | Bool | | false | |
| `sc_audition` | SC Audition | Bool | | false | |
| `delta` | Delta | Bool | | false | |
| `sc_lc_freq` | SC Low Cut Freq | Float | 20 to 20000 | 20.0 | Hz |
| `sc_lc_on` | SC Low Cut On | Bool | | false | |
| `sc_lc_slope` | SC Low Cut Slope | Choice | 6, 12, 24, 48, 96 dB/oct | 12 dB/oct |
| `sc_ls_freq` | SC Low Shelf Freq | Float | 20 to 20000 | 200.0 | Hz |
| `sc_ls_gain` | SC Low Shelf Gain | Float | -15 to 15 | 0.0 | dB |
| `sc_ls_on` | SC Low Shelf On | Bool | | false | |
| `sc_band_freq` | SC Band Freq | Float | 20 to 20000 | 1000.0 | Hz |
| `sc_band_gain` | SC Band Gain | Float | -15 to 15 | 0.0 | dB |
| `sc_band_q` | SC Band Q | Float | 0.1 to 10 | 1.0 | |
| `sc_band2_freq` | SC Band 2 Freq | Float | 20 to 20000 | 3000.0 | Hz |
| `sc_band2_gain` | SC Band 2 Gain | Float | -15 to 15 | 0.0 | dB |
| `sc_band2_q` | SC Band 2 Q | Float | 0.1 to 10 | 1.0 | |
| `sc_hs_freq` | SC High Shelf Freq | Float | 20 to 20000 | 8000.0 | Hz |
| `sc_hs_gain` | SC High Shelf Gain | Float | -15 to 15 | 0.0 | dB |
| `sc_hs_on` | SC High Shelf On | Bool | | false | |
| `sc_hc_freq` | SC High Cut Freq | Float | 20 to 20000 | 20000.0 | Hz |
| `sc_hc_on` | SC High Cut On | Bool | | false | |
| `sc_hc_slope` | SC High Cut Slope | Choice | 6, 12, 24, 48, 96 dB/oct | 12 dB/oct |

Sidechain frequency parameters use logarithmic skew (0.25) for natural frequency distribution.

### Stereo (2 parameters)

| ID | Name | Type | Range/Values | Default |
|----|------|------|--------------|---------|
| `stereo_link` | Stereo Link | Float | 0 to 100 | 100.0 % |
| `stereo_mode` | Stereo Mode | Choice | Normal, Mid, Side, M>S, S>M | Normal |

### MIDI / Sync (4 parameters)

| ID | Name | Type | Values | Default |
|----|------|------|--------|---------|
| `midi_trigger` | MIDI Trigger | Bool | | false |
| `host_sync` | Host Sync | Bool | | false |
| `sync_div` | Sync Division | Choice | 1/1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/4 D, 1/8 D, 1/4 T, 1/8 T | 1/4 |
| `sync_shape` | Sync Shape | Choice | Square, Triangle, Sine, Sawtooth | Square |

### Display / Global (5 parameters)

| ID | Name | Type | Default |
|----|------|------|---------|
| `analyzer_on` | Analyzer | Bool | true |
| `analyzer_pre` | Analyzer Pre | Bool | false |
| `oversample` | Oversampling | Choice (1x, 2x, 4x, 8x, 16x, 32x) | 1x |
| `compact_mode` | Compact Mode | Bool | false |
| `meter_scale` | Meter Scale | Choice (9, 18, 36, 60, 90 dB) | 60 dB |

## Parameter Architecture

### Source Files

| File | Purpose |
|------|---------|
| `parameters/ParameterIDs.h` | String ID constants (`ParamIDs::kThreshold`, etc.) |
| `parameters/ParameterLayout.h/cpp` | APVTS layout factory with ranges and defaults |
| `parameters/CompressorParameterAttachment.h` | Lock-free audio-thread parameter bridge |
| `utils/Constants.h` | Numeric range constants (`kMinThreshold`, `kMaxThreshold`, etc.) |

### Lock-Free Parameter Access

The `CompressorParameterAttachment` class caches `std::atomic<float>*` pointers from the APVTS. On the audio thread, `getCurrentParameters()` performs 51 atomic loads into a `CompressorParameters` struct with zero allocation:

```cpp
CompressorParameters p;
p.threshold = rawThreshold->load();
p.ratio     = rawRatio->load();
// ... etc
```

This struct is then passed to `CompressorEngine::updateParameters()`.

### Adding a New Parameter

1. Add a string ID constant in `ParameterIDs.h`
2. Add min/max/default constants in `Constants.h`
3. Add the parameter to the APVTS layout in `ParameterLayout.cpp`
4. Add an `std::atomic<float>*` member to `CompressorParameterAttachment`
5. Wire it in `init()` and `getCurrentParameters()`
6. Add the corresponding field to `CompressorParameters`
7. Use the value in `CompressorEngine::updateParameters()`
