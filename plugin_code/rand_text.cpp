/***************************

  * plugin模版v0.1

  **  注意事项：所有通过stdout流输出的内容都会作为输出消息

***************************/
#define LINUX_V
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string>
const std::string PLUGIN_NAME="rand_text";                                      //插件名称
const std::string PLUGIN_PATH="./plugin/"+PLUGIN_NAME;                        //插件路径，运行中产生的文件请保存到这个目录下
std::string plugin_function(std::string);
int main(int a,char**ss){
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
#include"../opstring.h"
#include<time.h>
#include<random>
#include<chrono>
using namespace std;
string txxts[1<<14];
int read_text(string name){
  int cnt=0;
  string fnm=PLUGIN_PATH+"/"+name+".txt";
  FILE*fp=fopen(fnm.c_str(),"r");
  if(fp==NULL)return -1;
  char temp_str[1<<12];
  while(fgets(temp_str,1<<12,fp)!=NULL){
    txxts[cnt++]=temp_str;
  }return cnt;
}
std::string plugin_function(std::string msgs){
  //char opt[1<<8]={0};
  string opt="";
  msgs=msg_decode(msgs);
  int sl=get_st(msgs.c_str(),"#");
  int sr=get_st(msgs.c_str(),"~");
  int ll=msgs.length();
  char name[1<<6]={0};
  char time[5]={0};
  get_copy(sl+1,sr,msgs.c_str(),name);
  get_copy(sr+1,ll,msgs.c_str(),time);
  int num=read_text(name);
  if(string(time)=="[NONE]"||num==-1)return "ERROR";
  int t;
  sscanf(time,"%d",&t);
  if(t<=0||t>10)return "ERROR";
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  uniform_int_distribution<int> u(0,num-1);
  default_random_engine e(seed);
  for(int i=0;i<t;i++){
    opt=opt+txxts[u(e)]+"\n";
  }



  return opt;
}
