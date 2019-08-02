#!/bin/sh
adduser sshd -H -D
ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -N ""
ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ""
echo /sbin/sshd >> /etc/init.d/rcS 
chown root /var/empty

