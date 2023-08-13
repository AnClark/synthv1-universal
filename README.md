# synthv1-universal - cross-platform port of synthv1

synthv1 is an old-school 4-oscillator subtractive polyphonic synthesizer with stereo fx. Written by [rncbc](https://github.com/rncbc) (a.k.a. Rui Nuno Capela).

Originally, synthv1 is for Linux only, providing a standalone JACK application, and a LV2 plugin. The latter is also available on Windows, but few DAWs support LV2 there (currently only REAPER, Ardour and MixBus).

This project aims at porting synthv1 to more common platforms and plugin formats, with the great power of DISTRHO Plugin Framework (DPF). I believe more musicians will enjoy this amazing synthesizer.

> **NOTE:** This project superceded [my fork of synthv1 with DPF implementation](https://github.com/AnClark/synthv1/tree/dpf-implementation). Any newer features and bugfixes will only be applied here.

## Features

- Cross-platform support
  - Windows
  - Linux
  - macOS _(to be tested)_
- Multi plugin formats
  - VST 2.4
  - VST3
  - LV2
  - CLAP

## Requirements

- Qt framework, C++ class library and tools for cross-platform application and UI development, <https://qt.io/>
- DISTRHO Plugin Framework, cross-platform unified audio plugin framework, <https://github.com/distrho/dpf>

## Source code structure

- `plugin/`: The main implementation. This is the core of the project.
- `src/`: Official synthv1 repository. Serves as a Git submodule.
- `dpf/`: DISTHRO Plugin Framework repository. Serves as a Git submodule.

## Principle

This project is based on DPF, for its multi-platform support. DPF provides a full solution for audio plugins, from base structures to final plugin binaries. You can build plugins for multi platforms and formats from just one source tree.

This project separates DPF port codes from official synthv1 repository, and uses its own CMake build rules. As synthv1 is in active development, this design helps keeping pace to rncbc's updates easily (by simply a `git pull` in `src/` directory).

## How to build

### On Linux

#### 0. Install dependencies

Make sure you have the latest Qt 6.x installed. If not, install it. Refer to the document of your distribution.

```bash
# Ubuntu
sudo apt install qt6-base-dev

# Arch Linux
sudo pacman -S qt6-base
```

#### 1. Clone source code

This project has submodules, so make sure you don't omit `--recursive`:

```bash
git clone https://github.com/<to_be_named> synthv1 --recursive
```

If you forget, run the following command:

```bash
git submodule update --init --recursive
```

#### 2. Configure and build

```bash
cd synthv1
cmake -S . -B build
cmake --build build
```

#### 3. Run plugin

Built plugin resides in `synthv1/build/bin`. Copy them to you DAW's plugin scan paths.

## Notice

- Standalone JACK support is not provided in this project. Go to official repo if you need it.
- macOS should be supported on both Intel and Apple Silicon platform, as long as you have the right Qt 6.x development package installed (e.g. [by MacPorts](https://ports.macports.org/port/qt6/)). But I don't own a MacBook. Please help me test the plugin and give me a report.

## License

This project is licensed under GNU GPL v2.
