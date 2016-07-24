#!/bin/bash

export TOOLCHAIN_NAME="sh4-oe-linux"
export PATH="/mnt/new2/openatv/build-enviroment/builds/openatv/spark/tmp/sysroots/i686-linux/usr/bin/sh4-oe-linux":$PATH
export SYSROOT="/mnt/new2/openatv/build-enviroment/builds/openatv/spark/tmp/sysroots/spark"

export CROSS_COMPILE=$TOOLCHAIN_NAME"-"

OPENSSL_PATH="/mnt/new2/new_openssl_sh4/out"
I1="-I$OPENSSL_PATH/include"
L1="-L$OPENSSL_PATH/lib"

export CFLAGS=" $I1 $L1 -fdata-sections -ffunction-sections -Os --sysroot=$SYSROOT "
export LDFLAGS=" $I1 $L1 -Wl,--gc-sections"



