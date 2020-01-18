#!/bin/sh
while [ true ]
do
    echo 2 > /sys/devices/platform/i2c-gpio.0/i2c-0/i2c-3/3-0039/sii9136_reconnect/sii9136_reconnect_sel
    sleep 1
done