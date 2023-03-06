#!/bin/sh
aarch64-linux-gnu-g++ -D_GLIBCXX_USE_CXX11_ABI=1 -std=c++11 -DLINUX_V -static Jiemeng.cpp -o ../Jiemeng -w