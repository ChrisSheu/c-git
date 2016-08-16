#!/bin/bash
set -e

#CROSS_COMPILE=""
#FOLDER=""
CC=gcc

if [ $# -ne 2 ]; then
    echo "Usage $0 [gcc or crosscompile] <folder>"
    echo "ex(none): $0 none ppp-2.4.4"
    echo "ex(arm): $0 arm-xxx-linux-gcc ppp-2.4.4"
    exit 1
fi

CROSS_COMPILE=$1
FOLDER=$2
cd $FOLDER

#./configure
if [ "$1" == "gcc" ]; then
    CC=$CC
else
    CC=$CROSS_COMPILE
fi
make CC=$CC clean all
