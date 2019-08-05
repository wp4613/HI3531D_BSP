#!/bin/bash

pushd ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/opensource/kernel/linux-3.18.y/
if [ -d ${TOP_DIRECTOR}/resource/kernel/${KERNEL_CONFIG} ];then
    cp ${TOP_DIRECTOR}/resource/kernel/${KERNEL_CONFIG} arch/arm/configs/
fi
if [ "x"$1 != "x" ];then
  case $1 in
    clean)
        make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- distclean
        exit 0
        ;;
    force)
        echo 'kernel distclean ...'
        make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- distclean
        ;;
    *)
        echo './build.sh clean|force'
  esac

fi

cp ${TOP_DIRECTOR}/resource/kernel/${KERNEL_CONFIG} arch/arm/configs/
make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- ${KERNEL_CONFIG}
make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- -j uImage
cp arch/arm/boot/uImage ${TOP_DIRECTOR}/build/out/



popd
