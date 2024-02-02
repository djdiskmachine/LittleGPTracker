#!/bin/bash
cd $(git rev-parse --show-toplevel)/projects/
BUILD="$(grep -oP 'BUILD_COUNT [^"]*"\K[^"]*' ../sources/Application/Model/Project.h)"
VERSION=1.3o_$BUILD
PACKAGE=LGPT-$VERSION.zip

collect_resources() { #1PLATFORM #2lgpt.*-exe
  if [[ -n $(find -name "$2") ]]; then
    echo "Build for $2!" # Found it
  else
    echo "-->file $2 not found<---"
    return;
  fi
  PACKAGE=LGPT-$1-$VERSION.zip
  echo "-=-=Packaging $PACKAGE=-=-"
  CONTENTS="./resources/$1/*"
  CONTENTS+=" $(find -iname $2)"
  if [ "$1" == "PSP" ]; then # PSP files go in the root folder
    zip -9 $PACKAGE -j $CONTENTS
  else # all the others go in the bin
    mkdir bin ; cp $CONTENTS bin
    zip -9 $PACKAGE bin/* && rm -r bin/
  fi
  cd ./resources/packaging 
  CONTENTS="README.txt samplelib/ lgpt_BETA/"
  zip -9 -r ../../$PACKAGE $CONTENTS
  CONTENTS="../../../docs/wiki/What-is-LittleGPTracker.md"
  CONTENTS+=" ../../../docs/wiki/config_xml.md"
  CONTENTS+=" ../$1/*.txt"
  zip -9 ../../$PACKAGE -j $CONTENTS && cd -
}

collect_resources PSP EBOOT.PBP
collect_resources DEB lgpt.deb-exe
# collect_resources RS97 lgpt.dge
# collect_resources BITTBOY lgpt-bittboy.elf
collect_resources MIYOO lgpt-miyoo.elf
# collect_resources STEAM lgpt.steam-exe
collect_resources W32 lgpt-W32.exe
collect_resources RASPI lgpt.rpi-exe
collect_resources CHIP lgpt.chip-exe