#!/bin/sh
# Build the cheat sheet.
#
# TEXMFVAR has to point somewhere writable.  The default on this machine
# (~/.texlive/texmf-var) is not, and without it pdflatex cannot generate the EC
# bitmap fonts and dies on the first \normalsize.  Running it twice resolves the
# page references.
set -e
cd "$(dirname "$0")"
export TEXMFVAR="$PWD/texmf/var"
export TEXMFCONFIG="$PWD/texmf/config"
mkdir -p "$TEXMFVAR" "$TEXMFCONFIG"
pdflatex -interaction=nonstopmode LittleGPTracker-Cheatsheet.tex > build.log 2>&1 || {
    echo "build failed:"; grep -E "^!" build.log | head; exit 1; }
pdflatex -interaction=nonstopmode LittleGPTracker-Cheatsheet.tex > build.log 2>&1 || true
grep -oE "Output written on .*" build.log
