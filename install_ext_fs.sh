#!/bin/bash
set -e

#CROSS_COMPILE=""
#FOLDER=""

if [ $# -ne 2 ]; then
    echo "Usage $0 [default or crosscompile] <folder>"
    echo "ex(none): $0 none e2fsprogs-1.41.11"
    echo "ex(arm): $0 arm-xxx-linux e2fsprogs-1.41.11"
    exit 1
fi

export CROSS_COMPILE=$1
FOLDER=$2
cd $FOLDER

if [ "$1" == "none" ]; then
    ./configure --exec-prefix=`pwd`/../install_$FOLDER --prefix=`pwd`/../install_$FOLDER --build=i686-pc-linux-gnu
else
    ./configure --host=$CROSS_COMPILE --exec-prefix=`pwd`/../install_$FOLDER --prefix=`pwd`/../install_$FOLDER --build=i686-pc-linux-gnu
fi

make
make install
make install-libs
