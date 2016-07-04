#!/bin/bash

PICOCOM="picocom"
VERSION="-1.4"
FOLDER=$PICOCOM$VERSION/

if [ $# -ne 1 ]; then
    echo "Usage $0 [none or crosscompile]"
    echo "ex(none): $0 none"
    echo "ex(arm): $0 arm-hisiv100nptl-linux-"
    exit 1
fi

if [ "$1" == "none" ]; then
    cd $FOLDER
    make clean picocom
else
    cd $FOLDER
    export CROSS_COMPILE=$1
    make clean picocom
fi
