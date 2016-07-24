#!/bin/bash

export TOOLCHAIN_NAME="arm-oe-linux-gnueabi"
BASE_PATH="/mnt/new2/openatv2/build-enviroment/builds/openatv/release/cube/tmp/sysroots/"
export PATH=$BASE_PATH"i686-linux/usr/bin/arm-oe-linux-gnueabi/":$PATH
export SYSROOT=$BASE_PATH"cube"

export CROSS_COMPILE=$TOOLCHAIN_NAME"-"

OPENSSL_PATH="/mnt/new2/armv5t/out"
I1="-I$OPENSSL_PATH/include"
L1="-L$OPENSSL_PATH/lib"

export CFLAGS=" $I1 $L1 -mfloat-abi=softfp -mtune=cortex-a9 -mfpu=vfpv3-d16 -pipe --sysroot=$SYSROOT "
export LDFLAGS=" $I1 $L1 "




