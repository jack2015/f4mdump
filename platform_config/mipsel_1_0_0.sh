#!/bin/bash

export TOOLCHAIN_NAME="mipsel-oe-linux"
export PATH="/mnt/new2/xspeedlx1/build-enviroment/builds/openatv/release/et4x00/tmp/sysroots/i686-linux/usr/bin/mipsel-oe-linux/":$PATH
export SYSROOT="/mnt/new2/xspeedlx1/build-enviroment/builds/openatv/release/et4x00/tmp/sysroots/et4x00"

export CROSS_COMPILE=$TOOLCHAIN_NAME"-"

export CFLAGS=" -fdata-sections -ffunction-sections -Os --sysroot=$SYSROOT "
export LDFLAGS=" -Wl,--gc-sections"



