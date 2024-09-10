#!/bin/sh
progdir=$(dirname "$0")/lgpt
cd $progdir
HOME=$progdir
LD_PRELOAD=./j2k.so ./lgpt-rg35xx.elf &> log.txt
sync
