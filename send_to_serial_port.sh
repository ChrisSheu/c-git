#!/bin/bash

RED='\033[33;31m'
GREEN='\033[33;32m'
YELLOW='\033[33;33m'
BLUE='\033[33;34m'
NC='\033[0m' # No Color
COLOR=$RED

if [ $# -ne 3 ]; then
    # not any device node parameters
    echo "Usage $0 <file> <device_node> <baud_rate>"
    # so print message
    exit 1
fi


stty -F $2 raw
stty -F $2 -echo -echoe -echok
stty -F $2 $3
sleep 1
while [ true ];
do
    while read line           
    do           
        #echo $line
        echo $line > $2
        sleep 0.5
    done <$1
done 
