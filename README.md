# Little Piggy Tracker

![Piggy](https://avatars.githubusercontent.com/u/180156201?s=400&u=ebb53bdea61a025edce0c3782ac75b532dd65dd7&v=4)

**Little Piggy Tracker** (f.k.a _'LittleGPTracker'_) is a music tracker optimised to run on portable game consoles. It is currently running on Windows, MacOS (intel/arm) & Linux, PSP, Miyoo Mini, and a collection of other retro gaming handhelds.

It implements the user interface of [littlesounddj](https://www.littlesounddj.com/lsd/index.php) and precedes [M8 tracker](https://www.dirtywave.com), two popular trackers greatly loved in the tracker community.

All versions are available for free under the [GPLv3 License](LICENSE). If you like the project and want to contribute, don't hesitate to make a pull request for this repo.

## About This Fork

This build is derivative of the work of the original author `m-.-n`
aka [Marc Nostromo](https://github.com/Mdashdotdashn/LittleGPTracker).
The original work and releasing the source code has laid the foundation for everything in this repo.

All implemented features have been tested not to break old
projects but make sure to backup your old cherished work
just to be safe &#9829;

## Releases

### Current Builds

Latest releases from this fork here:

- [Releases](https://github.com/djdiskmachine/LittleGPTracker/releases)


## Documentation

All the relevant documentation can be found in [Docs](docs) directory.

Recommended reading to get you started:

- [What is Little Piggy Tracker](docs/wiki/What-is-LittlePiggyTracker.md)
- [Quick-Start Guide](docs/wiki/quick_start_guide.md)
- [Little Piggy Tracker Configuration](docs/LittlePiggyTrackerConf.md)
- [Tips and Tricks](docs/wiki/tips_and_tricks.md)

## Features per platform

| Platform    | MIDI_Possible | MIDI_enabled | Soundfonts | Note                                 |
|-------------|---------------|--------------|------------|--------------------------------------|
| PSP         | NO            | NO           | YES        | [See notes](projects/resources/PSP/INSTALL_HOW_TO.txt) |
| DEB         | YES           | YES          | YES        |                                      |
| X64         | YES           | YES          | NO         |                                      |
| X86         | YES           | YES          | YES        |                                      |
| STEAM       | YES           | YES          | NO         |                                      |
| MIYOO       | NO            | NO           | YES        | Port by [Nine-H](https://ninethehacker.xyz) |
| W32         | YES           | YES          | YES        | Built in VS2008 with love            |
| RASPI       | YES           | YES          | YES        | Versatile platform                   |
| CHIP        | YES           | YES          | YES        | [See notes](projects/resources/CHIP/INSTALL_HOW_TO.txt) |
| BITTBOY     | MAYBE         | NO           | YES        |                                      |
| GARLIC      | MAYBE         | NO           | YES        | Port by [Simotek](http://simotek.net)|
| GARLICPLUS  | MAYBE         | NO           | YES        | Port by [Simotek](http://simotek.net)|
| RG35XXPLUS  | MAYBE         | NO           | YES        | Port by [Simotek](http://simotek.net)|
| MACOS       | YES           | YES          | NO         | Port by [clsource](https://genserver.social/clsource) |


* **Soundfont library is currently not ported for 64bit OS**
* **MIDI functionality __greatly__ depends on kernel support, please feature request your favourite OS maintainer =)**
* **Install ffmpeg by following install instructions for your platform [here](https://www.ffmpeg.org/download.html)**
* **PrintFX requires full ffmpeg. If marked as TBA, it requires a redesign using [libav](https://trac.ffmpeg.org/wiki/Using%20libav*)**
