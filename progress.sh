#!/bin/bash

count=1
interval=5
total=100

while true
do
    echo -ne '|'
    for i in $(seq $[total / interval]);
    do
        if [ $[count / interval] -ge $i ]
        then
            echo -ne "="
        else
            echo -ne " "
        fi
    done
    echo -ne '|'$count'/'$total'\r'

    if [ $count -eq $total ]
    then
        echo ""
        break
    fi

    count=$[count + 1]
    sleep 0.1
done
