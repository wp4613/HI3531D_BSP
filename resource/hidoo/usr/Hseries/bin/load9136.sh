#!/bin/sh

#gpio_16(6208-1 rst) - > H
#himm 0x120F01AC 0   #gpio_16 复用为gpio模式
#himm 0x12200400 1   #gpio_16 输出

fpga_done=0
while [ $fpga_done == 0 ]
do
    ./io_cfg.sh 11 5 "in"
    fpga_done=$?
    usleep 1000000
done

for line in $(grep "^0x" $1 |sed 's/\t/:/g')
do
    reg=$(echo $line | cut -d ":" -f1)
    val=$(echo $line | cut -d ":" -f2)
    echo "reg=$reg val=0x$val"
    ./i2c_test  -a $((0x72>>1))  -w -R $reg $val
done 

