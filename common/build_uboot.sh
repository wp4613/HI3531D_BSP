#!/bin/bash

pushd ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/opensource/uboot/u-boot-2010.06/

if [ "x"$1 != "x" ];then
  case $1 in
    clean)
        make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- distclean
        exit 0
        ;;
    force)
        echo 'uboot distclean ...'
        make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- distclean
        ;;
    *)
        echo './build.sh clean|force'
  esac

fi

make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- ${BOARD_UBOOT_CONFIG}
if [ "x${UBOOT_CONFIG_INC}" != "x" ];then
    cp ${TOP_DIRECTOR}/resource/uboot/${UBOOT_CONFIG_INC} include/configs/
fi
make ARCH=arm CROSS_COMPILE=${CROSS_COMPILER_PREFIX}- -j
cp u-boot.bin ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/pc/uboot_tools/
pushd ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/pc/uboot_tools/
cp ${TOP_DIRECTOR}/resource/uboot/${REG_INFO_BIN} ./reg_info.bin
chmod +x mkboot.sh
./mkboot.sh reg_info.bin u-boot-hi3531d.bin
if [ ! -d ${TOP_DIRECTOR}/build/out/ ];then
    mkdir ${TOP_DIRECTOR}/build/out/
fi
cp u-boot-hi3531d.bin  ${TOP_DIRECTOR}/build/out/
popd



popd
