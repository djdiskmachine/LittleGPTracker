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
  elif [ "$1" == "SWITCH" ]; then # everything lives in one SD card folder
    # hbmenu lists /switch/<dir>/<dir>.nro as a single entry, and the app
    # hardcodes sdmc:/switch/lgpt for both bin: and root:, so the binary and
    # its data - samplelib and the projects included - all go in that one
    # folder. The user merges "switch" into their SD card root.
    mkdir -p switch/lgpt ; cp $CONTENTS switch/lgpt
    mv switch/lgpt/$2 switch/lgpt/lgpt.nro
    cp -r ./resources/packaging/samplelib switch/lgpt
    # -type d so the lgpt_* project folders come across as folders, and so it
    # doesn't also pick up lgpt_package.sh sitting next to them
    find ./resources/packaging -maxdepth 1 -type d -name "lgpt_*" -exec cp -r {} switch/lgpt \;
    zip -9 -r $PACKAGE switch/ && rm -r switch/
  else # all the others go in the bin
    mkdir bin ; cp $CONTENTS bin
    zip -9 $PACKAGE bin/* && rm -r bin/
  fi
  cd ./resources/packaging
  # -j or these get stored as literal ../../../README.md, which unzip refuses
  zip -9 -jq ../../$PACKAGE ../../../README.md ../../../CHANGELOG ../../../LICENSE
  if [ "$1" != "SWITCH" ]; then # already staged inside switch/lgpt above
    CONTENTS="$(find . -name "samplelib" -type d)"
    CONTENTS+=" $(find . -name "lgpt_*" -type d)"
    zip -9 -r ../../$PACKAGE $CONTENTS
  fi
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
collect_resources GARLICPLUS lgpt-garlicplus.elf
collect_resources RG35XXPLUS lgpt-rg35xxplus.elf
collect_resources SWITCH lgpt-switch.nro
collect_resources MACOS LittleGPTracker.app
# collect_resources RS97 lgpt.dge
# collect_resources STEAM lgpt.steam-exe
