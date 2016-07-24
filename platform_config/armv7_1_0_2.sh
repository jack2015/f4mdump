#!/bin/bash

export TOOLCHAIN_NAME="arm-oe-linux-gnueabi"
export PATH="/mnt/new2/vusolo4k/openvuplus_3.0/build/vusolo4k/tmp/sysroots/i686-linux/usr/bin/arm-oe-linux-gnueabi/":$PATH
export SYSROOT="/mnt/new2/vusolo4k/openvuplus_3.0/build/vusolo4k/tmp/sysroots/vusolo4k"

export CROSS_COMPILE=$TOOLCHAIN_NAME"-"

OPENSSL_PATH="/mnt/new2/new_openssl_armv7/out"
I1="-I$OPENSSL_PATH/include"
L1="-L$OPENSSL_PATH/lib"

export CFLAGS=" $I1 $L1 -fdata-sections -ffunction-sections -Os -march=armv7-a -mfloat-abi=hard -mfpu=neon --sysroot=$SYSROOT "
export LDFLAGS=" $I1 $L1 -Wl,--gc-sections"



