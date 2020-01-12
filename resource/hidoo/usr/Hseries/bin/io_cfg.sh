#!/bin/sh

if [ $# == 0 ];then
    echo "example : $0 gpio_base gpio_number dir [val] "
    echo "example : $0 11 4 out 1 #(FPGA-program) " 
    echo "example : $0 11 6 out 1 #(6208-1 rst) "
    echo "example : $0 11 7 out 1 #(6208-2 rst) "

    exit 0
fi
case $1 in
    11)
        if [ $2 -ge 6 ];then
            let tmp=$2-6
            let GPIO_REG_MUX=0x120F01AC+4*tmp
        else 
            let GPIO_REG_MUX=0x120F0130+4*$2
        fi
        ;;
    15)
        if [ $2 -le 3 ];then
            GPIO_REG_MUX=$((0x120F02F4+4*$2))
            echo $GPIO_REG_MUX
        fi    
        ;;
    *)
        ;;    
    esac
if [ "x$GPIO_REG_MUX" == "x" ];then
    echo 暂不支持该GPIO
    exit 0
fi

if [ $3 == "out" ];then
    DIR=1
elif [ $3 == "in" ];then
    DIR=0
fi

if [ "x$DIR" == "x" ];then
    echo GPIO方向错误
    exit 0
fi
if [ $3 == 'in' ];then
    VAL=""
elif [ $4 -eq 1 ];then
    VAL=0xff
elif [ $4 -eq 0 ];then
    VAL=0
fi

let GPIO_REG_BASE=0x12150000+$1*0x10000
let GPIO_REG_DIR=$GPIO_REG_BASE+0x400
let tmp=$2+2
GPIO_REG_DATA=$(($GPIO_REG_BASE+(1<<($2+2)) )) #采用掩码方式单独操作一个gpio

#gpio_16(6208-1 rst) - > H
printf "GPIO_REG_MUX=%08x\\n" $GPIO_REG_MUX
printf "GPIO_REG_DIR=%08x\\n" $GPIO_REG_DIR
printf "GPIO_REG_DATA=%08X\\n" $GPIO_REG_DATA

himm $GPIO_REG_MUX  0   # 复用为gpio模式
tmp=$(himm $GPIO_REG_DIR | grep $GPIO_REG_DIR | cut -d ":" -f2)
if [ "$3" == "out" ];then
    tmp=$(($tmp| (1<<$2) ))
elif [ "$3" == "in" ];then
    tmp=$(($tmp& ~(1<<$2) ))
fi

    himm $GPIO_REG_DIR $tmp   #gpio_16 输出
if [ $3 == "out" ];then
    himm $GPIO_REG_DATA $VAL   #gpio_16 输出
else
    gpio_data=$(himm $GPIO_REG_DATA | grep "^$GPIO_REG_DATA:" | cut -d ":" -f2)
    #./io_cfg.sh 11 5 in | grep "^304087168" | cut -d ":" -f2
    #echo $(himm $GPIO_REG_DATA | grep "$GPIO_REG_DATA:" )
    #gpio_data=$($(himm $GPIO_REG_DATA) | grep "$GPIO_REG_DATA:" | cut -d ":" -f2)
    gpio_data=$(($gpio_data & $((1<<$2))))
    gpio_data=$(($gpio_data>>$2))
    echo $gpio_data
    return $gpio_data
    
fi
# himm 




