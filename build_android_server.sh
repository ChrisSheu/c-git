#!/bin/bash

arm-linux-androideabi-gcc -o output/server_android --sysroot=${NDK_SYSROOT} src/server.c
arm-linux-androideabi-strip output/server_android
