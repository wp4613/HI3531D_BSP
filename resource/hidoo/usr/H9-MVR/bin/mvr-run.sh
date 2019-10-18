#!/bin/sh
ulimit -d unlimited
ulimit -c unlimited

if [ -f /usr/H9-MVR/configs/upgrade.tar ];then
    cd /usr/H9-MVR/configs/
    tar xvf upgrade.tar
    if [ -d upgrade ];then
        if [ $? == 0 ];then
            cd upgrade
            chmod +x ./upgrade.sh
            ./upgrade.sh
            cd ../
            upgrade_done=true
        fi
        rm upgrade -rf
    fi
    if [ $upgrade_done ];then
        rm upgrade.tar -rf
    fi
fi

grep "^sshd" /etc/passwd >& /dev/null
if [ $? -ne 0 ]
then
    adduser sshd -H -D
fi
if [ ! -f "/etc/ssh/ssh_host_dsa_key" ];then
    ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -N ""
fi
if [ ! -f "/etc/ssh/ssh_host_rsa_key" ];then
    ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
fi
if [ -d "/ko/extdrv/nvp6134_ex.ko" ];then
    rm /ko/extdrv/nvp6134_ex.ko
fi
if [ -d "/ko/extdrv/sil9024_ex.ko" ];then
    rm /ko/extdrv/sil9024.ko
fi

chown root /var/empty
chmod 644 /var/empty/
/sbin/sshd


cd /usr/H9-MVR/bin/
./io_cfg.sh 11 4 out 1   #fpga program io
./io_cfg.sh 11 6 out 1 #(6208-1 rst)
./io_cfg.sh 11 7 out 1 #(6208-2 rst)

if [ ! -f /etc/init.d/S90mvr_init ];then
./i2c_test -a 0x70  -w 5    # i2c 选择 9136/6208-1
./load6208.sh /usr/H9-MVR/configs/CDCM6208_Settings_hidoo_H9_MVR_Y4_200MHz_Y3Y5Y6Y7_162.5MHz.ini
else
./load6208.sh /usr/H9-MVR/configs/CDCM6208_Settings_hidoo_H9_MVR_Y4_200MHz_Y3Y5Y6Y7_162.5MHz.ini cdcm6208-0 
fi
if [ ! -f /etc/init.d/S90mvr_init ];then
./i2c_test -a 0x70  -w 6    # i2c 选择 9136/6208-2
./load6208.sh /usr/H9-MVR/configs/CDCM6208_Settings_hidoo_H9_MVR_Y4-Y7_148.5MHz.ini
else
./load6208.sh /usr/H9-MVR/configs/CDCM6208_Settings_hidoo_H9_MVR_Y4-Y7_148.5MHz.ini cdcm6208-1 
fi


if [ ! -f /etc/init.d/S90mvr_init ];then
    ./i2c_test -a 0x70  -w 5    # i2c 选择 9136/6208-1
    ./io_cfg.sh 15 1 out 0  #(sil9136 rst)
    ./io_cfg.sh 15 1 out 1
    ./load9136.sh /usr/H9-MVR/configs/sil9136.reg &
fi
if [ -f /etc/init.d/S90mvr_init ];then
    fpga_load 0 /usr/H9-MVR/configs/mvr_fpga.bin 
fi

if [ -f /driver/hi_sil9136.ko ];then
    insmod /driver/hi_sil9136.ko
fi
/usr/H9-MVR/bin/guardian.sh /usr/H9-MVR/bin/start_video.sh >/dev/null 2>&1 &
/usr/H9-MVR/bin/guardian.sh "/usr/H9-MVR/bin/spi_master -t 1" > /dev/null 2>&1 &



