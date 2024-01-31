#!/bin/bash
mkdir build
cd build

g++ ../code/Jiemeng.cpp -lcurl -o ./Jiemeng


mkdir background
mkdir plugin
mkdir tmp
mkdir txt2img
mkdir deck
mkdir jm_script

g++ ../code/octave_launch.cpp -o ./jm_script/octave_launch

cp ../code/*.m jm_script
cp ../code/msyh.ttc txt2img
cp ../code/txt2img.py txt2img

