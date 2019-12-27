#!/bin/sh

NOVA_DRIVER_PATH=/usr/nova/driver
FPGA_CNT_MAX=100
FPGA_CFG_SLEEP=0.1
cnt=0

# start telnetd
/usr/sbin/telnetd &

#debugfs mount
mount -t debugfs none /sys/kernel/debug/

#insmod GPIO module
insmod ${NOVA_DRIVER_PATH}/hi_gpio.ko

#release reset of CDCM6208
# CDCM6208 U0(GPIO11_6)
echo 94 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio94/direction
echo 1 > /sys/class/gpio/gpio94/value
# CDCM6208 U1(GPIO11_7)
echo 95 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio95/direction
echo 1 > /sys/class/gpio/gpio95/value

#configure FPGA until finishing configuring
# FPGA DONE(GPIO11_5)
echo 93 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio93/direction
/usr/nova/bin/enable_fpga_cfg.sh
while [ $cnt -lt ${FPGA_CNT_MAX} ];do
	fpga_done=`cat /sys/class/gpio/gpio93/value`
	if [ ${fpga_done} -eq 1 ];then
		break;
	fi

	sleep ${FPGA_CFG_SLEEP}
	cnt=$(expr $cnt + 1)
done
# judge whether FPGA configured successfully
if [ $cnt -ge ${FPGA_CNT_MAX} ];then
	echo "FPGA configured failed."
else
	echo "FPGA configured OK."
fi

#install drivers
cd ${NOVA_DRIVER_PATH}
osmem=$(cat /proc/cmdline | cut -d ' ' -f 1 | cut -d '=' -f 2 | sed 's/M//g')
./load3531d_mvr_card -i -osmem $osmem
insmod hi_stub.ko
insmod hi_comif.ko
#insmod hi_fpga_cfg.ko
insmod hi_sysinit.ko
insmod hi_i2cdev.ko
insmod hi_sil9136.ko
insmod hi_spidev.ko
cd -

#other tasks

exit 0
