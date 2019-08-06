#!/bin/sh
adduser sshd -H -D
ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -N ""
ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
echo chmod +x ./load3531d && cd /ko/ && ./load3531d -i >> /etc/init.d/rcS 
echo /sbin/sshd >> /etc/init.d/rcS 
chown root /var/empty
rm /sbin/init-env.sh
sync
reboot

