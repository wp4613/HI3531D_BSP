#!/bin/sh
sh /usr/nova/bin/enable_arm_access.sh
SIZE=$(ls $1 -l|awk '{print $5}')
mtd_debug erase /dev/mtd3 0 $((($SIZE/(1024*1024)+1)*1024*1024))
mtd_debug write /dev/mtd3 0 $SIZE $1
sh /usr/nova/bin/enable_fpga_cfg.sh

