#!/bin/bash
set -e

# AS flags
ASFLAGS="-march=armv6j -mcpu=arm1176jz-s"

# Displays command line options
function usage() {
  echo "build-tests.sh source_file"
  exit
}

# Chooses the right toolchain
function choose_assembler() {
  if [ -x linux-gnueabi-as ]
  then
    AS=arm-linux-gnueabi-as
    OC=arm-linux-gnueabi-objcopy
  else
    AS=arm-none-eabi-as
    OC=arm-none-eabi-objcopy
  fi
}

# Compiles a source file
function assemble() {
  binary=${1%%.*}
  $AS $ASFLAGS $1 -o/tmp/${1##*/}.o
  $OC -O binary /tmp/${1##*/}.o $binary.o
}


# Checks arguments
if [ -r $1 ]
then
  choose_assembler && assemble $1
else
  echo "Invalid file"
fi
