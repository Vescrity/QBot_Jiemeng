#!/bin/bash

startTime=`date +%Y%m%d-%H:%M:%S`
startTime_s=`date +%s`

echo '尝试创建 build 目录...'

mkdir build
cd build

echo '开始执行编译'

cmake ..
make -j$(nproc) "$@"

echo '尝试构建工作目录'

mkdir tmp
mkdir deck
mkdir jm_script
mkdir Answer
mkdir luaclib
mkdir lualib

cp -r ../code/txt2img .
cp -r ../code/luarc .
cp -r ../sample .
cp ../LICENSE .
cp ./sample/config.json .
cp ../README.md .

endTime=`date +%Y%m%d-%H:%M:%S`
endTime_s=`date +%s`

sumTime=$[ $endTime_s - $startTime_s ]

echo "$startTime ---> $endTime" 
echo "本次构建耗时: $sumTime seconds"
