#!/bin/sh
progdir=$(dirname "$0")/lgpt
cd $progdir
HOME=$progdir
LD_LIBRARY_PATH=$PROGDIR:$LD_LIBRARY_PATH ./lgpt-rg35xxplus.elf &> ./lgpt.log
sync
