#!/bin/bash

pushd ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/opensource/kernel/linux-3.18.y/
if [ -d ${TOP_DIRECTOR}/resource/kernel/${KERNEL_CONFIG} ];then
    cp ${TOP_DIRECTOR}/resource/kernel/${KERNEL_CONFIG} arch/arm/configs/
fi
if [ $1 == "force" ];then
    make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- distclean
fi
make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- ${KERNEL_CONFIG}
make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- -j uImage
cp arch/arm/boot/uImage ${TOP_DIRECTOR}/build/out/



popd
