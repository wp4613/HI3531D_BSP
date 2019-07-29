#!/bin/sh
pushd build
if [ ! -d my_software_set ];then
    git clone https://github.com/zhangxuran11/my_software_set.git
    if [ $? != 0 -a -d my_software_set ];then
        rm my_software_set -r
    fi
fi
pushd my_software_set
./build.sh
popd

popd
