#!/bin/bash

set -o xtrace

function usage {
   echo "Usage:"
   echo "./make.sh sh4/sh4_1/sh4_1_0_2/mipsel/mipsel_1/mipsel_1_0_2/i686/i686_1/i686_1_0_2/armv7/armv7_1/armv7_1_0_2/armv5t_1_0_2 static"
   exit 1
}

function build {
    CROSS_COMPILE=$1
    PLATFORM=$2
    BIN_NAME=$3
    LIB_PATH=$4
    INC_PATH=$5
    CXX_ADD_PARAMS=$6
    CC_ADD_PARAMS=$7
    OPT="-Os"
    #OPT=" -g3 -O0"
    mkdir -p out/$PLATFORM
    rm -f ext/librtmp/*.o
    (cd ext/librtmp/ ; $CROSS_COMPILE"gcc" -c $OPT -Wall *.c $INC_PATH $LIB_PATH $CC_ADD_PARAMS)
    $CROSS_COMPILE"g++" $OPT -I inc/ -I ext/ -I ext/librtmp $INC_PATH $LIB_PATH  src/*.c src/*.cpp ext/librtmp/*.o -o out/$PLATFORM/$BIN_NAME -lssl -lcrypto -lz -std=c++0x $CXX_ADD_PARAMS $STATIC_LINK
    $CROSS_COMPILE"strip" -s out/$PLATFORM/$BIN_NAME
    rm -f ext/librtmp/*.o
    #-DDEBUG
}

PLATFORM=$1

if [ "$2" == "static" ]; then
    STATIC_LINK=" -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic " #-static-libstdc++
    BIN_POSTFIX="_static_libstdc++"
else
    STATIC_LINK=""
    BIN_POSTFIX=""
fi
    
case "$PLATFORM" in
        sh4)
            build "/opt/STM0/STLinux-2.4/devkit/sh4/bin/sh4-linux-" "sh4" "f4mdump_openssl.0.9.8$BIN_POSTFIX" "-L/opt/STM0/STLinux-2.4/devkit/sh4/lib" "" "$STATIC_LINK -Wno-narrowing "
            ;;
        sh4_1)
            build "/opt/STM1/STLinux-2.4/devkit/sh4/bin/sh4-linux-" "sh4" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "-L/opt/STM1/STLinux-2.4/devkit/sh4/lib" "" "$STATIC_LINK -Wno-narrowing "
            ;;
        sh4_1_0_2)
            source platform_config/sh4_1_0_2.sh
            build $CROSS_COMPILE "sh4" "f4mdump_openssl.1.0.2$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;
         
        mipsel)
            source platform_config/mipsel_0_9_8.sh
            build $CROSS_COMPILE "mipsel" "f4mdump_openssl.0.9.8$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;
        mipsel_1)
            source platform_config/mipsel_1_0_0.sh
            build $CROSS_COMPILE "mipsel" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;
        mipsel_1_0_2)
            source platform_config/mipsel_1_0_2.sh
            build $CROSS_COMPILE "mipsel" "f4mdump_openssl.1.0.2$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;
            
        armv7)
            build "arm-linux-gnueabihf-" "armv7" "f4mdump_openssl.0.9.8$BIN_POSTFIX" "-L/mnt/new2/armv7/openssl-0.9.8zh -L/usr/arm-linux-gnueabihf/lib/" "-I/mnt/new2/armv7/openssl-0.9.8zh/include" "$STATIC_LINK -Wno-narrowing "
            ;;
        armv7_1)
            build "arm-linux-gnueabihf-" "armv7" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "-L/mnt/new2/armv7/openssl-1.0.0t -L/usr/arm-linux-gnueabihf/lib/" "-I/mnt/new2/armv7/openssl-1.0.0t/include" "$STATIC_LINK -Wno-narrowing "
            ;;
        armv7_1_0_2)
            source platform_config/armv7_1_0_2.sh
            build $CROSS_COMPILE "armv7" "f4mdump_openssl.1.0.2$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;
            
        armv5t_1_0_2)
            source platform_config/armv5t.sh
            build $CROSS_COMPILE "armv5t" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;
         
        i686)
            build "" "i686" "f4mdump_openssl.0.9.8$BIN_POSTFIX" "-L/usr/local/lib" "" "$STATIC_LINK -Wno-narrowing "
            ;;
        i686_1)
            build "" "i686" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "-L/usr/local/lib" "" "$STATIC_LINK -Wno-narrowing "
            ;;  
        i686_1_0_2)
            source platform_config/i686_1_0_2.sh
            build "$CROSS_COMPILE" "i686" "f4mdump_openssl.1.0.2$BIN_POSTFIX" "$L1" "$I1" "$STATIC_LINK -Wno-narrowing $CFLAGS $LDFLAGS " " $CFLAGS $LDFLAGS "
            ;;            
        *)
            usage
            exit 1
esac

exit 
