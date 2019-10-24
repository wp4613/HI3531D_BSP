#!/bin/sh
if [ "x$VER1" == "x" ];then
    VER1=1.0.0.0
fi
if [ "x$VER2" == "x" ];then
    VER2=S1.T4.X1
fi
DATE=$(date "+%Y.%m.%d")
VERSION="$DATE H_AUX_9U V$VER1 $VER2"
if [ ! -f ./ver_tool ];then
    make
fi

./ver_tool "$VERSION"
