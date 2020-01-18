#!/bin/sh
cp $(ls . |grep -v upgrade.sh) / -r
if [ -d /usr/H9-MVR ];then
    rm -r /usr/H9-MVR
    ln -s /usr/Hseries /usr/H9-MVR
fi

if [ ! -f /sbin/ip ];then
    ln -s /bin/ip /sbin/ip
fi

if [ ! -d /var/db ];then
        mkdir /var/db -p
fi
