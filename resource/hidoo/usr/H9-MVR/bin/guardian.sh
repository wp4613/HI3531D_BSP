#!/bin/sh
cd /usr/H9-MVR/bin/
#while [ $(ps  |grep H9U_MVR_Card | wc -l) -lt 2 ]
while [ 1 ]
do
    ./H9U_MVR_Card 
    usleep 2000000
done
