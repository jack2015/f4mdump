#!/bin/bash


function usage {
   echo "Usage:"
   echo "./make.sh sh4/sh4_1/mipsel/mipsel_1/i686/i686_1"
   exit 1
}

function build {
    CROSS_COMPILE=$1
    PLATFORM=$2
    BIN_NAME=$3
    LIB_PATH=$4
    INC_PATH=$5
    mkdir -p out/$PLATFORM
    rm -f ext/librtmp/*.o
    (cd ext/librtmp/ ; $CROSS_COMPILE"gcc" -c -Os -Wall *.c $INC_PATH)
    $CROSS_COMPILE"g++" -Os -I inc/ -I ext/ -I ext/librtmp src/*.c src/*.cpp ext/librtmp/*.o -o out/$PLATFORM/$BIN_NAME $LIB_PATH -lssl -lcrypto -lz -static-libstdc++ -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic
    $CROSS_COMPILE"strip" -s out/$PLATFORM/$BIN_NAME
    rm -f ext/librtmp/*.o
    #-DDEBUG
}


if [ $# -ne 1 ]
then
    usage
fi

PLATFORM=$1
case "$PLATFORM" in
        sh4)
            build "/opt/STM0/STLinux-2.4/devkit/sh4/bin/sh4-linux-" "sh4" "f4mdump_openssl.0.9.8" "-L/opt/STM0/STLinux-2.4/devkit/sh4/lib" ""
            ;;
            
        sh4_1)
            build "/opt/STM1/STLinux-2.4/devkit/sh4/bin/sh4-linux-" "sh4" "f4mdump_openssl.1.0.0" "-L/opt/STM1/STLinux-2.4/devkit/sh4/lib" ""
            ;;
         
        mipsel)
            build "/opt/cross/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-" "mipsel" "f4mdump_openssl.0.9.8" "-L/opt/cross/mipsel-unknown-linux-gnu/lib" "-I/opt/cross/mipsel-unknown-linux-gnu/include"
            ;;

        mipsel_1)
            build "/opt/cross/mipsel-tuxbox-linux-gnu/bin/mipsel-tuxbox-linux-gnu-" "mipsel" "f4mdump_openssl.1.0.0" "-L/opt/cross/mipsel-tuxbox-linux-gnu/lib" "-I/opt/cross/mipsel-tuxbox-linux-gnu/include"
            ;;
         
        i686)
            build "" "i686" "f4mdump_openssl.0.9.8" "-L/usr/local/lib" ""
            ;;
        i686_1)
            build "" "i686" "f4mdump_openssl.1.0.0" "-L/usr/local/lib" ""
            ;;               
        *)
            usage
            exit 1
esac

exit 
