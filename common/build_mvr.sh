#/bin/sh
export ROOTFS=${TOP_DIRECTOR}/build/out/rootfs_mvr
if [  -d ${ROOTFS} ];then
    rm ${ROOTFS} -rf
fi

mkdir ${ROOTFS}

if [ ! -d ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/rootfs_uclibc/ ];then
    tar xvf ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/rootfs_uclibc.tgz -C ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/
fi



cp ${TOP_DIRECTOR}/resource/nova/rootfs/* ${ROOTFS}/ -rfd

#cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/rootfs_uclibc/* ${TOP_DIRECTOR}/build/out/rootfs/ -rfd
#cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/mpp/lib/* ${TOP_DIRECTOR}/build/out/rootfs/lib/ -rfd
#cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/mpp/ko ${TOP_DIRECTOR}/build/out/rootfs/ -rfd
cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/board/gdb/bin/bin/* ${ROOTFS}/bin/
cp ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/board/reg-tools-1.0.0/bin/* ${ROOTFS}/bin/ -rfd

cp ${TOP_DIRECTOR}/resource/hidoo/* ${ROOTFS}/ -rfd

#chmod +x ${TOP_DIRECTOR}/build/out/${ROOTFS}/ko/load3531d
mkdir ${ROOTFS}/var/empty
#cp ${TOP_DIRECTOR}/resource/init-env.sh ${TOP_DIRECTOR}/build/out/${ROOTFS}/sbin/


./build.sh compoment install
./build.sh yaffs2

