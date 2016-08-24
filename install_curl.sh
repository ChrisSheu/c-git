#!/bin/bash
set -e

ROOTDIR="${PWD}"
#CPPFLAGS="-I${ROOTDIR}/openssl/include -I${ROOTDIR}/zlib/include"
#LDFLAGS="-L${ROOTDIR}/openssl/libs -L${ROOTDIR}/zlib/libs"
#AR=${CROSS_COMPILE}-ar
#AS=${CROSS_COMPILE}-as
#LD=${CROSS_COMPILE}-ld
#RANLIB=${CROSS_COMPILE}-ranlib
#CC=${CROSS_COMPILE}-gcc
#NM=${CROSS_COMPILE}-nm
LIBS="-lssl -lcrypto"

if [ $# -ne 2 ]; then
    echo "Usage $0 [gcc or crosscompile] <folder>"                                                                                                             
    echo "ex(none): $0 gcc curl-7.37.1"
    echo "ex(arm): $0 arm-xxx-linux curl-7.37.1"
    exit 1
fi

CC=gcc
CROSS_COMPILE=$1
FOLDER=$2
OUTPUT_FOLDER=output_$FOLDER
cd $FOLDER

if [ "$CROSS_COMPILE" == "gcc" ]; then
    CROSS_COMPILE=""
    CC=$CC
else
    CROSS_COMPILE=$CROSS_COMPILE
    CC=$CROSS_COMPILE-$CC
fi

echo "===================="
echo "|  folder:$FOLDER  |"
echo "|  CROSS_COMPILE:$CROSS_COMPILE  |"
echo "|  CC:$CC  |"
echo "|  output:$OUTPUT_FOLDER  |"
echo "===================="
sleep 2
./configure --prefix=${ROOTDIR}/$OUTPUT_FOLDER \
            --target=${CROSS_COMPILE} \
            --host=${CROSS_COMPILE} \
            --build=i586-pc-linux-gnu \
            --with-ssl \
            --with-zlib


make CC=$CC clean install
