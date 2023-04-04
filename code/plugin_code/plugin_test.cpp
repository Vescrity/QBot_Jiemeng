/***************************

  * plugin模版v0.1

  **  注意事项：所有通过stdout流输出的内容都会作为输出消息

***************************/

#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<string>
const std::string PLUGIN_NAME="plugin_test";                                    //插件名称
const std::string PLUGIN_PATH=".\\plugin\\"+PLUGIN_NAME;                        //插件路径，运行中产生的文件请保存到这个目录下
std::string plugin_function(std::string);
int main(int a,char**ss){
  WinExec("chcp 65001",SW_HIDE);
  FILE*fp;
  fp=fopen("Current_msg.txt","r");
  int c;
  char ms[1<<13]={0};
  int i=0;
  while((c=fgetc(fp))!=EOF){
    ms[i++]=c;
  }
  printf("%s",plugin_function(std::string(ms)).c_str());
  return 0;
}
std::string plugin_function(std::string msgs){

  /***
    * code
  ***/

  return msgs;
}
