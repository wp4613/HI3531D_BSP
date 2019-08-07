#!/bin/sh
adduser sshd -H -D
ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -N ""
ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
rm /ko/extdrv/nvp6134_ex.ko
rm /ko/extdrv/sil9024.ko
echo "cd /ko/ && ./load3531d -i" >> /etc/init.d/rcS 
echo /sbin/sshd >> /etc/init.d/rcS 
echo "ulimit -d unlimited" >> /etc/init.d/rcS 
echo "ulimit -c unlimited" >> /etc/init.d/rcS 
chown root /var/empty
rm /sbin/init-env.sh
sync
reboot

