#!/bin/sh
echo_port=$(grep echo_port /usr/H9-MVR/configs/config.ini | cut -d "=" -f2)
mvr_port=$(grep mvr_port /usr/H9-MVR/configs/config.ini | cut -d "=" -f2)
/usr/H9-MVR/bin/H9U_MVR_Card 0 1 $mvr_port $echo_port 1> /dev/null  2>&1 
