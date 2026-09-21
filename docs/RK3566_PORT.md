# LittleGPTracker on RK3566 / ROCKNIX

Port of LittleGPTracker to RK3566 based handhelds running
[ROCKNIX](https://rocknix.org) — the Powkiddy RGB30, Anbernic RG-ARC, Gameforce
Chi and friends. It builds on an x86_64 host and deploys as a single executable:
no bundled libraries, no bundled fonts, no bundled C library.

## What the port consists of

| File | Purpose |
|------|---------|
| `projects/Makefile.RK3566` | Build configuration: toolchain, sysroot, defines |
| `projects/Makefile` | Two added lines registering `RK3566DIRS` / `RK3566FILES` |
| `projects/resources/RK3566/config.xml` | Fullscreen, panel fill, quit hotkey, auto-load, ayu dark theme |
| `projects/resources/RK3566/mapping.xml` | Gamepad button mapping |
| `projects/resources/RK3566/LittleGPTracker.sh` | Launch script for the EmulationStation "Ports" menu |
| `projects/resources/RK3566/setup-sysroot.sh` | Builds the cross sysroot from a device over ssh |
| `projects/resources/RK3566/fetch-demo-song.sh` | Installs a demo song from the official release |

Four changes were needed in shared code, each marked with a comment:

* `sources/Adapters/SDL2/Audio/SDLAudioDriver.cpp` — added the missing
  `<string.h>`: the file uses `memcpy`/`memmove` and relied on a transitive
  include that newer libstdc++ headers no longer provide.
* `sources/Adapters/SDL2/GUI/SDLGUIWindowImp.cpp` — drawing and presentation are
  now separate. The tracker draws its 320x240 screen at 1:1 into a fixed surface
  and `Flush()` scales that onto the window with `SDL_BlitScaled`. Previously the
  screen was drawn straight into the window at a whole-number multiple, which on
  a 720x720 panel capped the scale at 2x and produced unreadably small text (an
  8x8 glyph is about 1.6 mm on that display). See "Display" below.
* `sources/Adapters/SDL2/GUI/SDLGUIWindowImp.cpp` — opt-in `LGPT_SCREENSHOT`
  diagnostic (see below).
* `sources/Adapters/SDL2/GUI/SDLEventManager.{h,cpp}` — a configurable quit
  hotkey (`QUITBUTTONS`), and a fix for joystick event indexing. SDL2 reports a
  joystick *instance id* in `event.jbutton.which`, but the code used it directly
  as an index into four-element arrays; with a hotplugged second controller the
  instance id runs past the end of those arrays. Events are now mapped back to
  the index they were opened with.

## Target

| | |
|---|---|
| Device verified on | Powkiddy RGB30 |
| OS | ROCKNIX 20240815, kernel 6.9.12 |
| SoC | Rockchip RK3566, 4x Cortex-A55 (aarch64) |
| Display | 720x720 DSI panel, sway/Wayland compositor |
| Audio | PipeWire + PipeWire-Pulse, ALSA underneath |
| Gamepad | `retrogame_joypad` (rocknix-singleadc-joypad) on `/dev/input/js0` |
| glibc / libstdc++ | 2.38 / GCC 13 (`GLIBCXX_3.4.33`) |
| SDL2 | 2.30.5 |

ROCKNIX mounts `/` read-only from a squashfs image; `/storage` is the writable
ext4 partition. Ports therefore live under `/storage/roms/ports/`.

## How it builds

The tracker is cross-compiled but linked against the **target's own SDL2**. That
keeps the deliverable to one file and guarantees the app runs against exactly the
SDL2 the compositor and audio server were tested with. Two things are needed on
the build host:

1. **Arm GNU Toolchain** `aarch64-none-linux-gnu`, glibc **2.38**. The device
   exports `GLIBC_2.38` as its newest versioned symbol; a newer toolchain emits
   symbols the device's loader rejects.
2. **A sysroot** holding SDL2 2.30.x headers and the device's `libSDL2.so` for
   the linker. `projects/resources/RK3566/setup-sysroot.sh` assembles this over
   ssh:

```sh
cd projects/resources/RK3566
./setup-sysroot.sh /opt/rocknix-sysroot root@RK3566.lan
```

Then:

```sh
cd projects
make PLATFORM=RK3566 \
     RK3566_TOOLCHAIN=/opt/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu \
     RK3566_SYSROOT=/opt/rocknix-sysroot
```

The result is `projects/lgpt-rk3566.elf`. Build defines are `_64BIT`,
`CPP_MEMORY`, `SDL2`, `SDLAUDIO`, `DUMMYMIDI` and `_NO_JACK_`, compiled
`-std=gnu++03 -O3 -mcpu=cortex-a55`. `FFMPEG_ENABLED` is deliberately off: it
only gates the ffmpeg based sample importer, which needs a keyboard and file
dialog to be worth anything.

## How it deploys

```sh
DEST=/storage/roms/ports/LittleGPTracker
scp projects/lgpt-rk3566.elf \
    projects/resources/RK3566/config.xml \
    projects/resources/RK3566/mapping.xml \
    projects/resources/RK3566/LittleGPTracker.sh \
    root@RK3566.lan:$DEST/
ssh root@RK3566.lan "chmod +x $DEST/LittleGPTracker.sh $DEST/lgpt-rk3566.elf"
```

`config.xml` is read from the directory holding the executable, so it has to sit
next to the binary — not merely in the working directory. Projects and samples
are created in the working directory, which the launch script sets to the same
place.

## Input mapping

The RGB30's D-pad arrives as four ordinary buttons, not as a hat, so it is mapped
with `but:` rather than `hat:`. Button numbers are raw SDL joystick indices;
SDL numbers this pad in ascending evdev key code order, which matches
EmulationStation's own `es_input.cfg` for `retrogame_joypad`:

| Index | evdev | Physical |
|-------|-------|----------|
| 0 | `BTN_SOUTH` | B (bottom face button) |
| 1 | `BTN_EAST` | A (right face button) |
| 2 | `BTN_NORTH` | Y |
| 3 | `BTN_WEST` | X |
| 4 / 5 | `BTN_TL` / `BTN_TR` | L1 / R1 |
| 6 / 7 | `BTN_TL2` / `BTN_TR2` | L2 / R2 |
| 8 / 9 | `BTN_SELECT` / `BTN_START` | Select / Start |
| 10 | `BTN_MODE` | Guide |
| 11 / 12 | `BTN_THUMBL` / `BTN_THUMBR` | L3 / R3 |
| 13–16 | `BTN_DPAD_UP/DOWN/LEFT/RIGHT` | D-pad |

The left analog stick is mapped to the same four directions as a convenience.
Delete those four `joy:` lines from `mapping.xml` if stick drift ever produces
phantom presses.

To re-derive the numbers on different hardware, set `DUMPEVENT` to `YES` in
`config.xml`, press a button and read the `but(N):M` lines in `log.txt`, where
`M` is the button index.

### Quitting

**Hold Select + Start.** This is the only control that works from anywhere,
because the tracker's own quit control — the `Exit` button — lives on the
project picker and cannot be reached once a project is loaded, and a handheld
has no window to close. `config.xml` therefore sets:

```xml
<QUITBUTTONS value='8,9'/>
```

which is the SDL button index list that quits when held together, and

```xml
<AUTO_LOAD_LAST value='NO'/>
```

so the project picker (and with it a second way out) is always shown at
startup. Both are worth keeping unless you deliberately want to boot straight
into a project.

## Demo songs

All five demo songs from the official 1.6.0-bacon20 release are installed in the
port directory, so there is something to listen to out of the box. Pick one from
the project list after starting the tracker — the picker strips the `lgpt_`
prefix — and press Start.

| Demo | Song rows | Chains | Channels | Samples | Size |
|------|-----------|--------|----------|---------|------|
| `lgpt_Bootloop-FastJump` | 56 | 73 | 8 | 22 | 12 MB |
| `lgpt_Luce-COMPSENTRY` | 24 | 8 | 4 | 20 | 14 MB |
| `lgpt_Float-Fi-WildDope` | 16 | 29 | 8 | 15 | 8.5 MB |
| `lgpt_djdiskmachine-SereneDream` | 16 | 24 | 7 | 21 | 2.2 MB |
| `lgpt_BA0BAB_D1BBB5` | 8 | 25 | 7 | 6 | 2.1 MB |

`Bootloop-FastJump` is the most substantial of the five and the best place to
start. The row/chain/channel figures come from decoding the `SONG` hex blob in
each `lgptsav.dat`, which is `SONG_ROW_COUNT` x `SONG_CHANNEL_COUNT` bytes of
chain index, `0xFF` meaning empty.

Also installed is `samplelib`, the sample library the import browser opens in.
It appears in the project list too, because the picker lists every folder in the
port directory; it is not a project and will not load.

### One demo sample needed converting

Two of the songs shipped with a sample stored as **32-bit IEEE float** WAV
(RIFF format code 3): `anotherworld-chord_2.wav` in `Float-Fi-WildDope` and
`01 Paint You Blue - Ingrid Schroeder_3.wav` in `Luce-COMPSENTRY`. The tracker's
WAV reader only handles 8- and 16-bit PCM, so both failed to load and left one
instrument silent in each song.

Both were converted to 16-bit PCM at their original sample rate and channel
count, which the tracker reads correctly. That is the only modification made to
the released content. The originals remain in the release archive.

### Installing or restoring them

```sh
cd /storage/roms/ports/LittleGPTracker
/projects/resources/RK3566/fetch-demo-song.sh all      # every demo + samplelib
/projects/resources/RK3566/fetch-demo-song.sh lgpt_Float-Fi-WildDope
```

The demo projects are **not** committed to this repository. They are third party
musical works, and the tracker's `.gitignore` deliberately excludes `*.wav` and
`*.dat` — upstream drops them into `projects/resources/packaging/` at packaging
time rather than committing them. Fetching them from the project's own release
archive keeps the provenance and the licensing obvious. The script also works
from a host, and honours `LGPT_RELEASE_URL` for a local copy or a mirror.

## Display

The tracker draws a 320x240 screen. On a 720x720 panel there is no whole
multiple that both fills the screen and fits: 2x leaves 640x480 with borders all
round, 3x would need 960 pixels of width. Whole-multiple scaling therefore makes
the text far too small to read on this hardware, which is what the rewrite of
the SDL2 window code addresses — the drawn screen is scaled to the window with
`SDL_BlitScaled` instead.

With no `SCREENMULT` set, the screen **fills the panel**: 2.25x across and 3x
down, which makes the 8x8 font render at 18x24 physical pixels. The square panel
therefore stretches the original 4:3 layout vertically by a third.

```xml
<SCREENMULT value='2'/>
```

restores whole-multiple, aspect-correct presentation instead: exactly 2x,
centred, 640x480, with black borders. Smaller text, undistorted proportions.
There is no setting between the two — filling is the only way to use the full
height of a square panel.

## Theme

The port ships the **ayu dark** theme, set through the fifteen colour keys in
`config.xml`. Every value is taken from ayu itself and mapped by the role ayu
gives it, rather than by hue matching:

| Tracker key | ayu role | Value |
|-------------|----------|-------|
| `BACKGROUND` | `editor.background` | `10141C` |
| `FOREGROUND` | `editor.foreground` | `BFBDB6` |
| `CURSORCOLOR` | `editorCursor.foreground` | `E6B450` |
| `BORDER` | accent tint | `E6B450` |
| `ROWCOLOR1` | `editorLineNumber.activeForeground` | `5A6378` |
| `ROWCOLOR2` | `editorLineNumber.foreground` (`5a6378a6` over the background) | `404758` |
| `COL_TITLE` | `activityBarTop.foreground` | `697184` |
| `MAJORBEAT` | `activityBarTop.foreground` | `697184` |
| `SONGVIEW_00` | comment grey | `5A6673` |
| `SONGVIEW_FE` | token teal | `95E6CB` |
| `PLAYCOLOR`, `CONSOLE` | token green | `AAD94C` |
| `HICOLOR1` | token orange (keywords) | `FF8F40` |
| `HICOLOR2` | token yellow (functions) | `FFB454` |
| `MUTECOLOR` | token red | `F07178` |

Two points are deliberate. `BACKGROUND` is ayu's *editor* surface, not its
chrome: ayu reserves the darker `0D1017` for the sidebar, status bar and panels,
and the tracker's screen is the editing canvas. And `CURSORCOLOR` shares ayu's
single gold accent with the border, because ayu itself uses one accent for the
cursor, the border and active highlights.

Sources: [ayu-colors](https://github.com/ayu-theme/ayu-colors) and
[ayu for VS Code](https://github.com/ayu-theme/vscode-ayu), both MIT. Colours
are read once at startup, so a theme change needs a relaunch. Delete the colour
block from `config.xml` to return to the built-in purple theme.

## Checking what the tracker draws

The Mali GPU composites the display, so `/dev/fb0` stays blank and cannot be
used to confirm rendering. Instead the port can dump its own window surface:

```sh
LGPT_SCREENSHOT=/tmp/lgpt.bmp ./lgpt-rk3566.elf
```

The surface is written as a BMP on the first redraw and then periodically. It
works under every video driver, including headless ones, which makes it useful
for checking a build without taking over the device's screen:

```sh
SDL_VIDEODRIVER=dummy LGPT_SCREENSHOT=/tmp/lgpt.bmp timeout 5 ./lgpt-rk3566.elf
```

That only works when the executable is run directly. Going through
`LittleGPTracker.sh` sources `/etc/profile`, and
`/etc/profile.d/050-sway.conf` unconditionally forces
`SDL_VIDEODRIVER=wayland`, so a dummy capture has to bypass the launcher. The
same profile is what makes the launcher itself as short as it is: it supplies
`WAYLAND_DISPLAY`, `XDG_RUNTIME_DIR` and the SDL video/audio drivers, so the
script does not need to set up the environment at all.

## Verification performed

* Binary needs `GLIBC_2.38` and `GLIBCXX_3.4.32`; the device provides
  `GLIBC_2.38` and `GLIBCXX_3.4.33`.
* Launches on the device and stays running; the compositor reports a mapped
  full-panel window (`app_id=LittleGPTracker`, 720x720).
* The joystick is detected with 4 axes, 17 buttons and 0 hats, matching the
  evdev layout above, and all 13 mappings attach.
* Audio: an SDL probe requesting the exact settings the tracker uses
  (44100 Hz, `AUDIO_S16SYS`, stereo, 1024 samples, via `SDL_OpenAudio`) opens
  successfully as a PipeWire/PulseAudio stream on the device.
* Rendering: a headless capture of the app area is pixel-identical to an x86_64
  build of the same source, apart from the button that happened to be selected
  at the time.
* Display scaling: captures at both 1024x768 (dummy driver) and 720x720 (real
  Wayland session) confirm the whole panel is filled and the text is legible.
  Confirmed on the device by eye as well.
* Quit hotkey: `QUITBUTTONS=8,9` parses to mask `0x300` and the tracker logs
  `Quit combo mask 300` at startup.
* Demo songs: all five auto-load on the device with zero load errors and the
  expected sample counts (6, 28, 15, 21, 21). `Bootloop-FastJump` was played
  through to confirm audio: the log shows the project opening, its samples being
  read, and `[AUDIO] pulseaudio successfully opened with 1024 samples 44100` once
  playback starts, with a capture showing the song grid titled
  `Song: Bootloop-FastJump` and the play position advancing.
* Theme: a capture of the song screen was analysed pixel by pixel. Every
  specified ayu value is present in the rendered image, and no pixel remains from
  either the built-in purple palette or an earlier incorrect mapping.

## Troubleshooting

* **EmulationStation ends up sharing the screen.** ROCKNIX keeps ES running and
  expects whatever it launches to take the screen; sway tiles any window that
  does not ask to be fullscreen. If ES is left tiled beside another app, move the
  other one out of the way and ES fills the panel again:

  ```sh
  SWAYSOCK=/var/run/0-runtime-dir/sway-ipc.0.sock \
      swaymsg '[app_id="gmu.bin"] move to workspace 2'
  ```

  This was observed with the built-in Music Player (`gmu.bin`), which does not
  create a fullscreen window. It is unrelated to the tracker.
* **Cannot write to the deployed binary.** `scp` fails with `dest open ...
  Failure` while the tracker is running, because the kernel refuses to write a
  busy executable. Quit the tracker first (Select+Start).

## Known limitations

* MIDI is stubbed out (`DUMMYMIDI`). USB MIDI may work on ROCKNIX but is
  untested here.
* ffmpeg based sample import (`PrintFX`) is not compiled in.
* Filling a square panel stretches the 4:3 layout vertically; there is no
  scaling setting that both fills the screen and preserves the aspect ratio.
* The rendering comparison against an x86_64 build was made before the display
  rewrite. The scaling itself was verified by capture and by eye, not by
  byte-comparison against a reference.
* Verified on a Powkiddy RGB30 only. Other RK3566 ROCKNIX devices that use the
  same `retrogame_joypad` driver should behave identically, but the button
  indices in `mapping.xml` have not been confirmed on them.
