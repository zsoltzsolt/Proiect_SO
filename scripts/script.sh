#!/bin/bash

if test $# -ne 1
then
    echo "Invalid number of arguments"
else
    count=0
    while read line
    do
        if echo "$line" | grep -e "^[A-Z][A-Za-z0-9 ,\.\!\?]*" | 
        grep -e "[\!\?\.]$" | grep -ve ",[ ]*si" | grep -e "$1" > /dev/null
        then
            ((++count))
        fi
    done 
fi

echo $count

exit 8

#grep -e "[\!\?\.]$" | grep -ve ",[ ]*si"
