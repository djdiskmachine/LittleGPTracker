#!/bin/sh
# Check if libav dependency is installed
# Install it if not
if [ ! -f /usr/lib/libavutil.so ]; then
  cp lib/*so* /usr/lib/ &> lgpt.log
fi

progdir=$(dirname "$0")/lgpt
cd $progdir
HOME=$progdir
export LD_LIBRARY_PATH=$progdir/lib:$LD_LIBRARY_PATH
LD_PRELOAD=./j2k.so ./lgpt-rg35xx.elf &> lgpt.log
sync
