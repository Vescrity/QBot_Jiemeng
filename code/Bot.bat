windres -i Jiemeng.rc -o Jiemeng.o -F pe-x86-64
g++ -D_GLIBCXX_USE_CXX11_ABI=1 -std=c++11 -static Jiemeng.cpp Jiemeng.o -lwsock32 -fexec-charset=utf-8 -o ..\Jiemeng.exe 
cmd