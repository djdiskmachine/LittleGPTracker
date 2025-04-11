#!/bin/bash
cd "$(git rev-parse --show-toplevel)/projects/"
PROJECT_NUMBER=$(perl -nle 'print $1 if /PROJECT_NUMBER "([^"]*)"/' ../sources/Application/Model/Project.h)
PROJECT_RELEASE=$(perl -nle 'print $1 if /PROJECT_RELEASE "([^"]*)"/' ../sources/Application/Model/Project.h)
BUILD_COUNT=$(perl -nle 'print $1 if /BUILD_COUNT "([^"]*)"/' ../sources/Application/Model/Project.h)
VERSION="${PROJECT_NUMBER}.${PROJECT_RELEASE}.${BUILD_COUNT}"

collect_resources() { #1PLATFORM #2lgpt.*-exe
  if [[ -n $(find . -name "$2") ]]; then
    echo "Build for $2!" # Found it
  else
    echo "-->file $2 not found<---"
    return;
  fi
  PACKAGE=LGPT-$1-$VERSION.zip
  echo "-=-=Packaging $PACKAGE=-=-"
  BINARY=" $(find . -iname $2)"
  chmod +x $BINARY
  CONTENTS="./resources/$1/*"
  CONTENTS+=" ./custom_font.xml"
  CONTENTS+=" $BINARY"
  if [ "$1" == "PSP" ] ||
  [ "$1" == "GARLIC" ] ||
  [ "$1" == "RG35XXPLUS" ] ||
  [ "$1" == "BITTBOY" ]; then # All files go in the root folder
    zip -9 $PACKAGE -j $CONTENTS
  elif [ "$1" == "MACOS" ]; then # .app is a folder
    zip -9 $PACKAGE -j $CONTENTS
    zip -9yr $PACKAGE LittleGPTracker.app/
  else # all the others go in the bin
    mkdir bin ; cp $CONTENTS bin
    zip -9 $PACKAGE bin/* && rm -r bin/
  fi
  cd ./resources/packaging 
  CONTENTS="../../../README.md ../../../CHANGELOG ../../../LICENSE samplelib/ lgpt_BETA/"
  zip -9 -r ../../$PACKAGE $CONTENTS
  CONTENTS="../../../docs/wiki/What-is-LittlePiggyTracker.md"
  CONTENTS+=" ../../../docs/wiki/config_xml.md"
  CONTENTS+=" ../../../docs/wiki/tips_and_tricks.md"
  CONTENTS+=" ../$1/*.txt"
  zip -9 ../../$PACKAGE -jq $CONTENTS && cd -
}

collect_resources PSP EBOOT.PBP
collect_resources DEB lgpt.deb-exe
collect_resources X64 lgpt.x64
collect_resources X86 lgpt.x86
collect_resources MIYOO lgpt-miyoo.elf
collect_resources W32 lgpt-W32.exe
collect_resources RASPI lgpt.rpi-exe
collect_resources CHIP lgpt.chip-exe
collect_resources BITTBOY lgpt-bittboy.elf
collect_resources GARLIC lgpt-garlic.elf
collect_resources GARLICPLUS lgpt-garlicplus.elf
collect_resources RG35XXPLUS lgpt-rg35xxplus.elf
collect_resources MACOS LittleGPTracker.app
# collect_resources RS97 lgpt.dge
# collect_resources STEAM lgpt.steam-exe
