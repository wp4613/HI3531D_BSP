#!/bin/bash
which ${CROSS_COMPILER_PREFIX}-gcc
if [ $? != 0 ];then
    if [ -d ${CROSS_COMPILER_PREFIX} ];then
        rm ${CROSS_COMPILER_PREFIX} -rf
    fi
    tar -xvf ${CROSS_COMPILER_PREFIX}.tgz
    pushd ${CROSS_COMPILER_PREFIX}
    sudo ./${CROSS_COMPILER_PREFIX}.install
    echo please input : source /etc/profile
    popd
    rm ${CROSS_COMPILER_PREFIX} -rf
fi

if [ ! -d ${HI3531D_SDK} ];then
    tar -xvf ${HI3531D_SDK}.tgz
    pushd ${HI3531D_SDK}
    sed  -i "s/\/bin\/sh/\/bin\/bash/g" ./sdk.unpack
    ./sdk.unpack
    pushd osdrv/tools/
    make
    popd # osdrv/tools/
    popd #${HI3531D_SDK} 
fi
