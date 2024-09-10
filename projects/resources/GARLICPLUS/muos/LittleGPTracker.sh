#!/bin/sh
echo app >/tmp/act_go

. /opt/muos/script/var/device/storage.sh

export LD_LIBRARY_PATH=/usr/lib32

PROGDIR="$DC_STO_ROM_MOUNT/MUOS/application/.lgpt"
cd $PROGDIR
HOME=$PROGDIR

echo "lgpt" >/tmp/fg_proc

LD_LIBRARY_PATH=$PROGDIR:$LD_LIBRARY_PATH ./lgpt-rg35xxplus.elf &> lgpt.log
sync
