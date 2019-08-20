#/bin/sh
export ROOTFS=${TOP_DIRECTOR}/build/out/rootfs

./build.sh busybox
./build.sh compoment install
./build.sh yaffs2
