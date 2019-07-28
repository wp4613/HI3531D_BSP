#!/bin/bash
sh_exec=$(ls -lh /bin/sh | awk '{print $11}')
if [ "x"$sh_exec != "xbash" ];then
    sudo rm /bin/sh
    sudo ln -s /bin/bash /bin/sh
fi
source ./local.cfg
which ${CROSS_COMPILER_PREFIX}-gcc
if [ $? == 0 ];then
    SYSROOT=$(${CROSS_COMPILER_PREFIX}-gcc -print-sysroot)
fi
case $1 in
    init)
        echo ' start init...'
        common/build_init.sh
        ;;
    make)
        echo ' start make...'
        common/build_make.sh
        ;;
    clean)
        rm ${HI3531D_SDK} -rf
        ;;
    *)
        echo './build.sh init|make'
esac

