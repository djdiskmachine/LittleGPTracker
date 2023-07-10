#!/bin/sh
PROGDIR=`dirname "$0"`
cd $PROGDIR

LD_LIBRARY_PATH=$PROGDIR:$LD_LIBRARY_PATH ./lgpt.elf > ./lgpt.log
