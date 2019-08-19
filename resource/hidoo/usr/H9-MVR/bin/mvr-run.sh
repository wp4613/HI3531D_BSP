#!/bin/sh
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
/sbin/sshd

cd /usr/H9-MVR/bin/
./i2c_test -a 0x70  -w 5    # i2c 选择 9136/6208-1
./io_cfg.sh 15 1 out 0  #(sil9136 rst)
./io_cfg.sh 15 1 out 1
./load9136.sh /usr/H9-MVR/configs/sil9136.reg &
