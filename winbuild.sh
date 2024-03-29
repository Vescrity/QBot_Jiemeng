#!/bin/bash
mkdir winbuild
cd winbuild

# g++ ../code/Jiemeng.cpp -lcurl -o ./Jiemeng
x86_64-w64-mingw32-g++ ../code/Jiemeng.cpp -lws2_32 -o ./Jiemeng.exe  --static --std=c++20

mkdir background
mkdir plugin
mkdir tmp
mkdir txt2img
mkdir deck
mkdir jm_script
mkdir Answer

# g++ ../code/octave_launch.cpp -o ./jm_script/octave_launch

cp ../code/*.m jm_script
cp ../code/msyh.ttc txt2img
cp ../code/txt2img.py txt2img
cp ../README.md .
cp ../进阶说明.md .
cp ../LICENSE .
cp -r ../sample .
cp ./sample/config.json .

