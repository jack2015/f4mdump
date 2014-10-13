#!/bin/bash


function usage {
   echo "Usage:"
   echo "./make.sh sh4/mipsel/i686"
   exit 1
}

function build {
    CROSS_COMPILE=$1
    PLATFORM=$2
    mkdir -p out/$PLATFORM
	$CROSS_COMPILE"g++" -g -I inc/ src/*.cpp src/*.c -o out/$PLATFORM/f4mdump
	$CROSS_COMPILE"strip" out/$PLATFORM/f4mdump
}

if [ $# -ne 1 ]
then
    usage
fi

PLATFORM=$1
case "$PLATFORM" in
        sh4)
            build "/opt/STM/STLinux-2.4/devkit/sh4/bin/sh4-linux-" "sh4"
            ;;
         
        mipsel)
            build "/opt/cross/mipsel-unknown-linux-gnu/bin/mipsel-unknown-linux-gnu-" "mipsel"
            ;;
         
        i686)
            build "" "i686"
            ;;         
        *)
            usage
            exit 1
esac
