#!/bin/bash

pushd ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/tools/pc/ubi_sh/
chmod +x mkubiimg.sh
./mkubiimg.sh hi35xx 2k 128k ${TOP_DIRECTOR}/build/out/rootfs 230M ${TOP_DIRECTOR}/build/${HI3531D_SDK}/osdrv/pub/bin/pc 1
cp *.ubifs ${TOP_DIRECTOR}/build/out/


popd
