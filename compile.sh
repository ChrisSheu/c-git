#!/bin/sh
set -e

#CROSS_COMPILE="arm-xxxx-linux-"
#FOLDER="busybox-1.14.1"

if [ $# -lt 1 ]; then
    echo "Usage $0 <folder> [crosscompile]"
    echo "cross-platform example: $0 folder arm-xxx-linux-"
    echo "default example: $0 folder"
    exit 1
fi

cd $1

make clean
if [ ! -z $2 ]; then
    make CROSS_COMPILE="$2" menuconfig
    make CROSS_COMPILE="$2" defconfig
    make CROSS_COMPILE="$2"
else
    make menuconfig
    make defconfig
    make
fi

cp busybox ../
ls -alF ../busybox
