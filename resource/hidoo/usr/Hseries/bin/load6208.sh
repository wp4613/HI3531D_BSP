#!/bin/sh

#gpio_16(6208-1 rst) - > H
#himm 0x120F01AC 0   #gpio_16 复用为gpio模式
#himm 0x12200400 1   #gpio_16 输出
if [ $# -lt 1 ];then
    echo "#0 config_file dev_name"
    exit 1
fi

CONF_FILE=$1
if [ $# -gt 1 ];then
    DEV="-d $2"
fi

echo "./i2c_master  $DEV -a 0x54 -D -w -R 0 0x0"
CDCM6208LoadCfg()
{
    for line in $(grep "^Register " $1 | sed   's/Register //g' |  awk '{printf $1 ;print $2}')
    do
        reg=$(echo $line | cut -d ":" -f1)
        val=$(echo $line | cut -d ":" -f2)
        if [ $reg == 21 ];then
            break
        fi
        echo "reg=$reg val=0x$val"
        ./i2c_master  $DEV -a 0x54 -D -w -R $reg 0x$val
    done 
    CDCM6208SoftReset
}

CDCM6208SoftReset()
{
    tmp=0x$(./i2c_master $DEV -a 0x54 -D -R 0x0003 | grep "read data:" | cut -d ":" -f2 | sed 's/ *//g')
    ./i2c_master $DEV -a 0x54 -D  -R 0x0003 -w $(($tmp & ~0x0040))
    usleep 20
    ./i2c_master $DEV -a 0x54 -D  -R 0x0003 -w $tmp
}


CDCM6208LoadCfg $1
tmp=0x$(./i2c_master $DEV  -a 0x54 -D -R 0x15 | grep "read data:" | cut -d ":" -f2 | sed 's/ *//g')
if [ $(($tmp & 0x0004)) != 0 ];then
    CDCM6208LoadCfg $1
fi
