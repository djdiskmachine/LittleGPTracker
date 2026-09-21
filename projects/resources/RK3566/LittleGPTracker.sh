#!/bin/bash
#
# LittleGPTracker / Little Piggy Tracker - ROCKNIX (RK3566) launch script
#
# This is the script EmulationStation shows in the "Ports" menu.  Everything
# the tracker needs - the binary, config.xml and mapping.xml - sits next to it
# in this directory, and the tracker keeps its projects and samples here too.
#
# Ported by cross-compiling against the device's own SDL2; see
# docs/RK3566_PORT.md in the source tree for how to rebuild it.

# ROCKNIX's /etc/profile.d/050-sway.conf already exports WAYLAND_DISPLAY,
# XDG_RUNTIME_DIR, SDL_VIDEODRIVER=wayland and SDL_AUDIODRIVER=pulseaudio,
# which is exactly the environment the tracker needs.  Do not second-guess it
# here: anything exported before this line gets overwritten anyway.
. /etc/profile

# Resolve this script's directory and work from there: the tracker treats the
# current directory as "root:", where projects and samples are read from and
# written to.  HOME is left alone, so PulseAudio and SDL keep their state in
# /storage rather than scattering dotfiles through the port directory.
progdir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$progdir" || exit 1

# The tracker logs to stdout; capture it beside the binary for troubleshooting.
rm -f log.txt
./lgpt-rk3566.elf > log.txt 2>&1

# The tracker writes projects to the SD card; make sure they land before
# EmulationStation takes the screen back.
sync
