#!/bin/bash


function usage {
   echo "Usage:"
   echo "./make.sh sh4/sh4_1/mipsel/mipsel_1/i686/i686_1 static"
   exit 1
}

function build {
    CROSS_COMPILE=$1
    PLATFORM=$2
    BIN_NAME=$3
    LIB_PATH=$4
    INC_PATH=$5
    CPP_ADD_PARAMS=$6
    OPT="-Os"
    #OPT=" -g3 -O0"
    mkdir -p out/$PLATFORM
    rm -f ext/librtmp/*.o
    (cd ext/librtmp/ ; $CROSS_COMPILE"gcc" -c $OPT -Wall *.c $INC_PATH)
    $CROSS_COMPILE"g++" $OPT -I inc/ -I ext/ -I ext/librtmp src/*.c src/*.cpp ext/librtmp/*.o -o out/$PLATFORM/$BIN_NAME $LIB_PATH -lssl -lcrypto -lz -std=c++0x $CPP_ADD_PARAMS $STATIC_LINK
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
         
        mipsel)
            build "/opt/cross/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-" "mipsel" "f4mdump_openssl.0.9.8$BIN_POSTFIX" "-L/opt/cross/mipsel-unknown-linux-gnu/lib" "-I/opt/cross/mipsel-unknown-linux-gnu/include" "$STATIC_LINK "
            ;;

        mipsel_1)
            build "/opt/cross/mipsel-tuxbox-linux-gnu/bin/mipsel-tuxbox-linux-gnu-" "mipsel" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "-L/opt/cross/mipsel-tuxbox-linux-gnu/lib" "-I/opt/cross/mipsel-tuxbox-linux-gnu/include" "$STATIC_LINK -Wno-narrowing "
            ;;
         
        i686)
            build "" "i686" "f4mdump_openssl.0.9.8$BIN_POSTFIX" "-L/usr/local/lib" "" "$STATIC_LINK -Wno-narrowing "
            ;;
        i686_1)
            build "" "i686" "f4mdump_openssl.1.0.0$BIN_POSTFIX" "-L/usr/local/lib" "" "$STATIC_LINK -Wno-narrowing "
            ;;               
        *)
            usage
            exit 1
esac

exit 
