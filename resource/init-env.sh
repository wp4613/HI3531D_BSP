#!/bin/sh
adduser sshd -H -D
ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -N ""
ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
rm /ko/extdrv/nvp6134_ex.ko
rm /ko/extdrv/sil9024.ko
echo "cd /ko/ && ./load3531d -i" >> /etc/init.d/rcS 
echo /sbin/sshd >> /etc/init.d/rcS 
chown root /var/empty

echo "pushd /root/" >> /etc/init.d/rcS
echo "./i2c_test CDCM6208_Settings_H9_MVR_hidoo.ini" >> "/etc/init.d/rcS"
echo "popd" >> "/etc/init.d/rcS"

rm /sbin/init-env.sh

sync
reboot

