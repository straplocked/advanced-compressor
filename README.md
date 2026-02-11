# Advanced Compressor

A professional-grade dynamics compressor audio plugin built with [JUCE 7](https://juce.com/), inspired by FabFilter Pro-C 2. Features 8 compression styles, a 3-band sidechain EQ, mid/side processing, oversampling, lookahead, and a real-time spectrum analyzer.

Sister plugin to [Advanced EQ](../advanced-eq).

## Features

- **8 Compression Styles** -- Clean, Classic, Opto, Vocal, Mastering, Bus, Punch, Pumping -- each with distinct detection modes, ballistics, and character
- **Soft-Knee Compression** with adjustable knee width (0-72 dB) and range limiting
- **3-Band Sidechain EQ** -- low cut, band (bell), high cut with full frequency control
- **External Sidechain** input with audition mode
- **Mid/Side Processing** -- compress mid or side independently
- **Stereo Link** -- 0-100% continuously variable between dual-mono and linked
- **Lookahead** up to 20 ms with automatic latency compensation
- **Oversampling** -- 1x, 2x, or 4x for reduced aliasing on aggressive settings
- **Auto Gain** makeup compensation
- **Dry/Wet Mix** -- 0-200% range for parallel compression
- **Real-Time Spectrum Analyzer** with pre/post display
- **19 Factory Presets** across Vocal, Drums, Bus, Mastering, and Creative categories
- **Input/Output Gain and Pan** controls

## Plugin Formats

| Format | Platform |
|--------|----------|
| VST3 | macOS, Windows, Linux |
| AU | macOS |
| CLAP | macOS, Windows, Linux (requires submodule) |
| Standalone | macOS, Windows, Linux |
| AAX | macOS, Windows (requires SDK) |

## Building

### Requirements

- CMake 3.25+
- C++20 compiler (Clang 14+, GCC 12+, MSVC 2022+)
- JUCE 7.0.12 (included as git submodule)

### macOS

```bash
git clone --recurse-submodules https://github.com/yourname/advanced-compressor.git
cd advanced-compressor
cmake -B build
cmake --build build
```

Builds universal binary (arm64 + x86_64). Plugins are automatically copied to:
- AU: `~/Library/Audio/Plug-Ins/Components/`
- VST3: `~/Library/Audio/Plug-Ins/VST3/`

### Windows

```bash
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

### Linux

```bash
sudo apt-get install libasound2-dev libfreetype6-dev libx11-dev libxrandr-dev libxcursor-dev libxinerama-dev
cmake -B build
cmake --build build
```

### CLAP Support

To build with CLAP format, initialize the submodule:

```bash
git submodule update --init libs/clap-juce-extensions
```

### AAX Support

```bash
cmake -B build -DAAX_SDK_PATH=/path/to/aax-sdk
cmake --build build
```

## Project Structure

```
advanced-compressor/
├── CMakeLists.txt                  Build configuration
├── cmake/PlatformDefaults.cmake    Platform compiler flags
├── JUCE/                           JUCE framework (submodule)
├── libs/clap-juce-extensions/      CLAP support (optional submodule)
├── docs/                           Documentation
├── resources/                      Fonts, icons, presets
└── source/
    ├── PluginProcessor.h/cpp       Audio processor entry point
    ├── PluginEditor.h/cpp          GUI editor
    ├── dsp/                        DSP engine
    ├── analyzer/                   FFT spectrum analyzer
    ├── metering/                   Level and GR metering
    ├── parameters/                 Parameter definitions
    ├── presets/                    Preset system
    ├── ui/                        UI components and styling
    └── utils/                     Math utilities and constants
```

See [docs/](docs/) for detailed documentation.

## Parameters

30 automatable parameters across 6 groups:

| Group | Parameters |
|-------|-----------|
| **Dynamics** | Threshold, Ratio, Knee, Range, Attack, Release, Hold, Lookahead |
| **Style** | Compression style (8 choices) |
| **Gain/Mix** | Input/Output Gain, Input/Output Pan, Wet Gain, Auto Gain, Mix |
| **Sidechain** | External, Audition, LC Freq/On, Band Freq/Gain, HC Freq/On |
| **Stereo** | Stereo Link, Stereo Mode |
| **Global** | Analyzer On/Pre, Oversampling, Compact Mode |

## Compression Styles

| Style | Detection | Topology | Character |
|-------|-----------|----------|-----------|
| **Clean** | Peak | Feedforward | Low distortion, program-dependent release |
| **Classic** | RMS | Feedback | Vintage, very program-dependent |
| **Opto** | RMS | Feedforward | Slow, non-linear two-stage release |
| **Vocal** | Peak+RMS | Feedforward | Auto knee/ratio, brings vocals forward |
| **Mastering** | RMS | Feedforward | Maximum transparency |
| **Bus** | RMS | Feedback | Glue character |
| **Punch** | Peak | Feedforward | Fast, preserves transients |
| **Pumping** | Peak | Feedforward | Exaggerated envelope, deep pumping |

## License

All rights reserved.

## Tech Stack

- **Framework**: JUCE 7.0.12
- **Language**: C++20
- **Build**: CMake 3.25+
- **DSP**: Cytomic TPT SVF filters, one-pole envelope followers
- **Threading**: Lock-free audio thread via atomic parameters and triple buffers
