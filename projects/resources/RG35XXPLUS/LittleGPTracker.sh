#!/bin/bash
progdir=$(dirname "$0")/lgpt
cd $progdir
HOME=$progdir
./lgpt-rg35xxplus.elf &> log.txt
sync
