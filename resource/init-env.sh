#!/bin/sh
adduser sshd -H -D
ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -N ""
ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
rm /ko/extdrv/nvp6134_ex.ko
rm /ko/extdrv/sil9024.ko

rm /sbin/init-env.sh

sync
reboot

