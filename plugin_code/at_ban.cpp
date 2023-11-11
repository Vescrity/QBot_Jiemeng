/***************************

  * plugin模版v0.1

  **  注意事项：所有通过stdout流输出的内容都会作为输出消息

***************************/

#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<string>
const std::string PLUGIN_NAME="at_ban";                                         //插件名称
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
#include "..\opstring.h"
std::string plugin_function(std::string msgs){

  int l,r;
  l=get_st(msgs.c_str(),"qq=");
  l+=3;
  r=get_st(msgs.c_str()+l,"]")+l;
  char qids[16]={0};
  get_copy(l,r,msgs.c_str(),qids);
  string opts="[chgcrt:.type_repost#[group_id]#";
  opts=opts+qids+"#[ban:10]]";

  return opts;
}
