#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<Winbase.h>
#include<string>
using namespace std;
/*****此程序用于执行消息发送命令******/
int main(int a,char**str){
  string comm("curl \"");
  comm=comm+str[2]+"\"";
  WinExec(comm.c_str(),SW_HIDE);
}
