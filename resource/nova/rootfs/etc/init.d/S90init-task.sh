#!/bin/sh

NOVA_DRIVER_PATH=/usr/nova/driver

# start telnetd
/usr/sbin/telnetd &

#debugfs mount
mount -t debugfs none /sys/kernel/debug/

#insmod GPIO module
insmod ${NOVA_DRIVER_PATH}/hi_gpio.ko

#release reset of CDCM6208
#/usr/nova/bin/himm 0x12200400 0xc0
#/usr/nova/bin/himm 0x12200300 0xc0 
# CDCM6208 U0(GPIO11_6)
echo 94 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio94/direction
echo 1 > /sys/class/gpio/gpio94/value
# CDCM6208 U1(GPIO11_7)
echo 95 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio95/direction
echo 1 > /sys/class/gpio/gpio95/value

#release reset of SIL9136
#/usr/nova/bin/himm 0x12240400 0x2
#/usr/nova/bin/himm 0x12240008 0x2

#install drivers
cd ${NOVA_DRIVER_PATH}
osmem=$(cat /proc/cmdline  | cut  -d ' ' -f 1 | cut -d '=' -f 2 | sed 's/M//g')
./load3531d_mvr_card -i -osmem $osmem
insmod hi_stub.ko
insmod hi_comif.ko
insmod hi_fpga_cfg.ko
insmod hi_sysinit.ko
insmod hi_i2cdev.ko
insmod hi_sil9136.ko
insmod hi_spidev.ko
cd -

#other tasks

exit 0
