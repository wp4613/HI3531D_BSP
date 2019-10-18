#!/bin/sh

#mount /dev/mtdblock3 /mnt/
#ifconfig eth0 192.168.1.101

#release reset of CDCM6208
himm 0x12200400 0xc0
himm 0x12200300 0xc0 

#release reset of SIL9136
himm 0x12240400 0x2
himm 0x12240008 0x2

cd /driver
osmem=$(cat /proc/cmdline  | cut  -d ' ' -f 1 | cut -d '=' -f 2 | sed 's/M//g')
./load3531d_mvr_card -i -osmem $osmem

insmod hi_stub.ko
insmod hi_comif.ko
insmod hi_gpio15.ko
insmod hi_gpio11.ko
insmod hi_fpga_cfg.ko
insmod hi_sysinit.ko
insmod hi_i2cdev.ko
#insmod hi_sil9136.ko
insmod hi_spidev.ko


cd -
