#/bin/sh
if [ $# -lt 3 ];then                                               
    echo not version                                          
    echo Useage:                                                                                                                                       
    echo '  ./build.sh mvr "H_AUX_9U 1.0.0.0 S2.T1" '
    exit 1                                                          
fi
DEV_V=$1
VER1_V=$2
VER2_V=$3

export ROOTFS=${TOP_DIRECTOR}/build/out/rootfs_mvr
if [  -d ${ROOTFS} ];then
    rm ${ROOTFS} -rf
fi

mkdir ${ROOTFS}

if [ ! -d ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/rootfs_uclibc/ ];then
    tar xvf ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/rootfs_uclibc.tgz -C ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/
fi



cp ${TOP_DIRECTOR}/resource/nova/rootfs/* ${ROOTFS}/ -rfd
rm ${ROOTFS}/usr/nova/bin/himm

#cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/rootfs_uclibc/* ${TOP_DIRECTOR}/build/out/rootfs/ -rfd
#cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/mpp/lib/* ${TOP_DIRECTOR}/build/out/rootfs/lib/ -rfd
#cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/mpp/ko ${TOP_DIRECTOR}/build/out/rootfs/ -rfd

cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/board/gdb/bin/bin/* ${ROOTFS}/bin/
cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/board/reg-tools-1.0.0/bin/* ${ROOTFS}/bin/ -rfd

rm ${ROOTFS}/etc/init.d/S80network

cp ${TOP_DIRECTOR}/resource/hidoo/* ${ROOTFS}/ -rfd
cp ${TOP_DIRECTOR}/resource/hidoo_third_soft/* ${ROOTFS}/ -rfd

pushd tools/ver_tool/
#VER1_V=$(cat ${TOP_DIRECTOR}/resource/hidoo/usr/H9-MVR/configs/version.txt |grep VER1|cut -d '=' -f2)
#VER2_V=$(cat ${TOP_DIRECTOR}/resource/hidoo/usr/H9-MVR/configs/version.txt |grep VER2|cut -d '=' -f2)
DEV=$DEV_V VER1=$VER1_V VER2=$VER2_V ./create_ver.sh
cp ./mcu_version.bin ./fpga_version.bin
cp ./mcu_version.bin ./version.bin
cp ./*version.bin ${ROOTFS}/usr/H9-MVR/configs/
popd

#chmod +x ${TOP_DIRECTOR}/build/out/${ROOTFS}/ko/load3531d
#mkdir ${ROOTFS}/var/empty
#cp ${TOP_DIRECTOR}/resource/init-env.sh ${TOP_DIRECTOR}/build/out/${ROOTFS}/sbin/


#./build.sh compoment install
./build.sh yaffs2

pushd ${TOP_DIRECTOR}/build/out/
tar cvf rootfs_mvr.tar rootfs_mvr
popd

./build.sh mvr_upgrade
