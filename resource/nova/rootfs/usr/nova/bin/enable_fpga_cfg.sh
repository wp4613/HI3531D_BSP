#!/bin/sh

GPIO_PATH=/sys/class/gpio

# HI3531D is disabled to access SPI NOR FLASH
if [ ! -d ${GPIO_PATH}/gpio90 ];then
	echo 90 > ${GPIO_PATH}/export
fi
echo out > ${GPIO_PATH}/gpio90/direction
echo 1 > ${GPIO_PATH}/gpio90/value

# release FPGA
if [ ! -d ${GPIO_PATH}/gpio92 ];then
	echo 92 > ${GPIO_PATH}/export
fi
echo out > ${GPIO_PATH}/gpio92/direction
echo 0 > ${GPIO_PATH}/gpio92/value
sleep 0.01
echo 1 > ${GPIO_PATH}/gpio92/value
