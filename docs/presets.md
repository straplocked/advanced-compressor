# Presets

## Factory Presets

19 built-in presets across 5 categories:

### Vocal (4)

| Preset | Threshold | Ratio | Knee | Attack | Release | Style | Mix |
|--------|-----------|-------|------|--------|---------|-------|-----|
| Vocal Smooth | -24 dB | 3:1 | 8 dB | 15 ms | 150 ms | Vocal | 100% |
| Vocal Aggressive | -18 dB | 6:1 | 4 dB | 5 ms | 80 ms | Vocal | 100% |
| Vocal Parallel | -30 dB | 8:1 | 6 dB | 8 ms | 120 ms | Clean | 50% |
| Vocal De-Esser | -28 dB | 4:1 | 3 dB | 0.5 ms | 40 ms | Clean | 100% |

### Drums (4)

| Preset | Threshold | Ratio | Knee | Attack | Release | Style | Mix |
|--------|-----------|-------|------|--------|---------|-------|-----|
| Drum Snap | -20 dB | 4:1 | 6 dB | 0.1 ms | 80 ms | Punch | 100% |
| Drum Smash | -25 dB | 10:1 | 3 dB | 1 ms | 50 ms | Clean | 100% |
| Drum Room Crush | -35 dB | 20:1 | 0 dB | 0.5 ms | 200 ms | Pumping | 40% |
| Kick Tighten | -16 dB | 3:1 | 6 dB | 5 ms | 60 ms | Clean | 100% |

### Bus (4)

| Preset | Threshold | Ratio | Knee | Attack | Release | Style | Mix |
|--------|-----------|-------|------|--------|---------|-------|-----|
| Bus Glue | -22 dB | 2.5:1 | 10 dB | 20 ms | 200 ms | Bus | 100% |
| Mix Bus Gentle | -18 dB | 2:1 | 12 dB | 30 ms | 300 ms | Bus | 100% |
| Drum Bus Punch | -20 dB | 4:1 | 8 dB | 10 ms | 100 ms | Punch | 100% |
| Guitar Bus | -24 dB | 3:1 | 8 dB | 15 ms | 150 ms | Classic | 100% |

### Mastering (3)

| Preset | Threshold | Ratio | Knee | Attack | Release | Style | Mix |
|--------|-----------|-------|------|--------|---------|-------|-----|
| Mastering Transparent | -16 dB | 1.5:1 | 15 dB | 30 ms | 400 ms | Mastering | 100% |
| Mastering Warm | -18 dB | 2:1 | 12 dB | 25 ms | 300 ms | Classic | 100% |
| Mastering Loud | -12 dB | 3:1 | 8 dB | 20 ms | 200 ms | Mastering | 100% |

### Creative (4)

| Preset | Threshold | Ratio | Knee | Attack | Release | Style | Mix |
|--------|-----------|-------|------|--------|---------|-------|-----|
| Pumping EDM | -20 dB | 8:1 | 0 dB | 0.5 ms | 250 ms | Pumping | 100% |
| Sidechain Duck | -30 dB | 100:1 | 0 dB | 0.1 ms | 200 ms | Clean | 100% |
| NY Compression | -35 dB | 10:1 | 6 dB | 5 ms | 80 ms | Clean | 40% |
| Opto Leveler | -22 dB | 3:1 | 10 dB | 20 ms | 500 ms | Opto | 100% |

## Preset Data Structure

Each preset stores a `CompressorPresetData` struct:

```cpp
struct CompressorPresetData
{
    float threshold;    // dB
    float ratio;        // :1
    float knee;         // dB
    float range;        // dB
    float attackMs;     // ms
    float releaseMs;    // ms
    float holdMs;       // ms
    int   style;        // CompressorStyle enum index
    float mix;          // 0-200%
    float outputGain;   // dB
};
```

## User Presets

Users can save and manage custom presets. Presets are stored as XML files in the platform-appropriate directory:

| Platform | Location |
|----------|----------|
| macOS | `~/Library/Application Support/AdvancedCompressor/Presets/` |
| Windows | `%APPDATA%/AdvancedCompressor/Presets/` |
| Linux | `~/.config/AdvancedCompressor/Presets/` |

### PresetManager API

```cpp
// Browse
getCategories()                    // Returns list of category names
getPresetsInCategory(category)     // Returns presets in a category
nextPreset() / previousPreset()    // Navigate with wrapping

// Apply
applyPreset(index)                 // Load preset into APVTS

// Save/Delete
saveCurrentAsPreset(name)          // Save current state as user preset
deleteUserPreset(index)            // Remove user preset from disk

// Notifications
addListener(Listener*)             // Get notified on preset changes
```

## Adding Factory Presets

Add presets to the appropriate function in `source/presets/FactoryPresets.cpp`:

```cpp
presets.push_back ({
    "Preset Name",        // name
    "Category",           // category
    {
        -20.0f,           // threshold (dB)
        4.0f,             // ratio
        6.0f,             // knee (dB)
        60.0f,            // range (dB)
        10.0f,            // attack (ms)
        100.0f,           // release (ms)
        0.0f,             // hold (ms)
        0,                // style index (0=Clean, 1=Classic, etc.)
        100.0f,           // mix (%)
        0.0f              // output gain (dB)
    }
});
```
