#/bin/sh
export ROOTFS=${TOP_DIRECTOR}/build/out/upgrade
if [  -d ${ROOTFS} ];then
    rm ${ROOTFS} -rf
fi
mkdir ${ROOTFS}

cp ${TOP_DIRECTOR}/resource/hidoo/* ${ROOTFS}/ -rfd
rm ${ROOTFS}/etc/init.d/S81network1
rm ${ROOTFS}/etc/init.d/S82network2
rm ${ROOTFS}/usr/nova/etc/fpga/mvr_fpga.bin

#mkdir ${ROOTFS}/usr/H9-MVR/configs/ -p
#mkdir ${ROOTFS}/bin/ -p
#cp ${TOP_DIRECTOR}/resource/hidoo/usr/H9-MVR/configs/version.txt ${ROOTFS}/usr/H9-MVR/configs/
echo "#!/bin/sh" > ${ROOTFS}/bin/hellow.sh
echo "echo hellow" > ${ROOTFS}/bin/hellow.sh
chmod +x ${ROOTFS}/bin/hellow.sh

chmod +x ${TOP_DIRECTOR}/resource/upgrade.sh
cp ${TOP_DIRECTOR}/resource/upgrade.sh ${ROOTFS}/ -rfd
pushd ${TOP_DIRECTOR}/build/out/
tar cvf upgrade.tar upgrade
popd

