#!/bin/sh

GPIO_PATH=/sys/class/gpio

# Hold FPGA in reset state
if [ ! -d ${GPIO_PATH}/gpio92 ];then
	echo 92 > ${GPIO_PATH}/export
fi
echo out > ${GPIO_PATH}/gpio92/direction
echo 0 > ${GPIO_PATH}/gpio92/value

# HI3531D is enabled to access SPI NOR FLASH
if [ ! -d ${GPIO_PATH}/gpio90 ];then
	echo 90 > ${GPIO_PATH}/export
fi
echo out > ${GPIO_PATH}/gpio90/direction
echo 0 > ${GPIO_PATH}/gpio90/value
