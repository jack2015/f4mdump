#!/bin/bash

export CROSS_COMPILE=""

OPENSSL_PATH="/mnt/new2/new_openssl_i686/out"
I1="-I$OPENSSL_PATH/include"
L1="-L$OPENSSL_PATH/lib"

export CFLAGS=" $I1 $L1 -fdata-sections -ffunction-sections -Os --sysroot=$SYSROOT "
export LDFLAGS=" $I1 $L1 -Wl,--gc-sections"



