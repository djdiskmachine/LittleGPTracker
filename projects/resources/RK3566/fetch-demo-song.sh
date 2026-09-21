#!/bin/bash
#
# Fetch a demo song and install it into the port directory.
#
# The demo songs ship inside the official LittleGPTracker release archives,
# alongside samplelib. They are not committed to this repository: the demo
# projects are third party musical works, and the tracker's .gitignore
# deliberately excludes *.wav and *.dat (upstream drops them into
# projects/resources/packaging/ at packaging time instead of committing them).
# Downloading them from the project's own release keeps the provenance obvious.
#
# Usage:
#   ./fetch-demo-song.sh [PROJECT] [DEST] [TAG]
#
#   PROJECT  which demo to install        (default: lgpt_Bootloop-FastJump)
#   DEST     port directory on the device (default: /storage/roms/ports/LittleGPTracker)
#   TAG      release to take it from      (default: 1.6.0-bacon20)
#
# Available demo songs in the 1.6.0-bacon20 release, with the size of the song
# grid and how much of it is used (8 channels):
#
#   lgpt_Bootloop-FastJump          56 rows, 73 chains, 8 channels   12 MB
#   lgpt_Luce-COMPSENTRY            24 rows,  8 chains, 4 channels   14 MB
#   lgpt_Float-Fi-WildDope          16 rows, 29 chains, 8 channels  8.5 MB
#   lgpt_djdiskmachine-SereneDream  16 rows, 24 chains, 7 channels  2.2 MB
#   lgpt_BA0BAB_D1BBB5               8 rows, 25 chains, 7 channels  2.1 MB
#
# Run this on the device (it has curl and unzip), or on a host with the port
# directory mounted. The whole release archive is ~78 MB, most of it samplelib,
# so the download is the slow part.
#
set -euo pipefail

PROJECT="${1:-lgpt_Bootloop-FastJump}"
DEST="${2:-/storage/roms/ports/LittleGPTracker}"
TAG="${3:-1.6.0-bacon20}"

URL="${LGPT_RELEASE_URL:-https://github.com/djdiskmachine/LittleGPTracker/releases/download/${TAG}/LGPT-X64-${TAG}.zip}"

if [ ! -d "$DEST" ]; then
    echo "port directory not found: $DEST" >&2
    exit 1
fi

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

echo "==> downloading $TAG (about 78 MB, this takes a while)"
curl -fSL --progress-bar -o "$WORK/lgpt.zip" "$URL"

echo "==> extracting $PROJECT"
if ! unzip -l "$WORK/lgpt.zip" "$PROJECT/*" >/dev/null 2>&1; then
    echo "no such demo project in $TAG: $PROJECT" >&2
    echo "available:" >&2
    unzip -l "$WORK/lgpt.zip" | awk '{print $4}' | grep -oE '^lgpt_[^/]+' | sort -u >&2
    exit 1
fi
unzip -q -o "$WORK/lgpt.zip" "$PROJECT/*" -d "$WORK"

echo "==> installing into $DEST"
rm -rf "${DEST:?}/$PROJECT"
cp -r "$WORK/$PROJECT" "$DEST/$PROJECT"

# Point the tracker at it, so enabling AUTO_LOAD_LAST later opens the demo
# rather than nothing.
printf '%s\n' "$DEST/$PROJECT" > "$DEST/last_project"

echo
echo "installed $(du -sh "$DEST/$PROJECT" | cut -f1):"
ls "$DEST/$PROJECT"
echo
echo "Start the tracker and pick '${PROJECT#lgpt_}' from the project list."
