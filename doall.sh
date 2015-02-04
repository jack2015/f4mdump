#!/bin/bash
set -e

SILLEN=">/dev/null 2>&1"

eval $PWD/make.sh sh4 "$SILLEN"
eval $PWD/make.sh sh4_1 $SILLEN
eval $PWD/make.sh mipsel $SILLEN
eval $PWD/make.sh mipsel_1 $SILLEN
eval $PWD/make.sh i686 $SILLEN
eval $PWD/make.sh i686_1 $SILLEN

echo ">>>>>>>>>>>>>>>>>>>>>>>> DONE <<<<<<<<<<<<<<<<<<<<<<<<"
exit 