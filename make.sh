#!/bin/bash


function usage {
   echo "Usage:"
   echo "./make.sh sh4/mipsel/i686"
   exit 1
}

function build {
    CROSS_COMPILE=$1
    PLATFORM=$2
    LIB_PATH=$3
    mkdir -p out/$PLATFORM
	$CROSS_COMPILE"g++" -I inc/ -I inc/librtmp src/*.c src/*.cpp -o out/$PLATFORM/f4mdump -L $LIB_PATH  -Wl,-Bstatic -lrtmp -Wl,-Bdynamic -lssl -lcrypto -lz
    $CROSS_COMPILE"strip" -s out/$PLATFORM/f4mdump
    #$CROSS_COMPILE"g++" -g -I inc/ -I inc/librtmp src/*.c src/*.cpp -o out/$PLATFORM/udsdump -o /home/sulge/e2/rtmp/new/rtmpdump/librtmp/librtmp.a
	#$CROSS_COMPILE"strip" out/$PLATFORM/udsdump
    #-lrtmp
}


if [ $# -ne 1 ]
then
    usage
fi

PLATFORM=$1
case "$PLATFORM" in
        sh4)
            build "/opt/STM/STLinux-2.4/devkit/sh4/bin/sh4-linux-" "sh4" "/opt/STM/STLinux-2.4/devkit/sh4/lib"
            ;;
         
        mipsel)
            build "/opt/cross/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-" "mipsel" "/opt/cross/mipsel-unknown-linux-gnu/lib"
            ;;
         
        i686)
            build "" "i686" "/usr/local/lib"
            ;;         
        *)
            usage
            exit 1
esac
