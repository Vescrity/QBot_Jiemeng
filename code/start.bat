windres -i start.rc -o start.o -F pe-x86-64
g++ -static start.cpp start.o -fexec-charset=utf-8 -o ..\start.exe 
cmd