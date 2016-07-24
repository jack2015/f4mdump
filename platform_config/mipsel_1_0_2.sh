#!/bin/bash

export TOOLCHAIN_NAME="mipsel-oe-linux"
export PATH="/mnt/new2/xspeedlx1/build-enviroment/builds/openatv/release/et4x00/tmp/sysroots/i686-linux/usr/bin/mipsel-oe-linux/":$PATH
export SYSROOT="/mnt/new2/xspeedlx1/build-enviroment/builds/openatv/release/et4x00/tmp/sysroots/et4x00"

export CROSS_COMPILE=$TOOLCHAIN_NAME"-"

OPENSSL_PATH="/mnt/new2/new_openssl_mipsel/out"
I1="-I$OPENSSL_PATH/include"
L1="-L$OPENSSL_PATH/lib"

export CFLAGS=" $I1 $L1 -fdata-sections -ffunction-sections -Os --sysroot=$SYSROOT "
export LDFLAGS=" $I1 $L1 -Wl,--gc-sections"



