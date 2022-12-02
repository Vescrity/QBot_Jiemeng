/*******主程序*******/
#include<stdio.h>
#include<string>
#include<time.h>
#include<iostream>
#include<stdlib.h>
#include<thread>
#include"Socket_Input.h"
#include"Jie_Meng_Base.h"
#include"Msg_type.h"
#include"Special_Judge.h"
#include"Key_Ans.h"
#include"File_Read.h"
#include"Daily.h"
#pragma comment (lib, "ws2_32.lib")
#define THR_NUM 128
using namespace std;
int thr_id;
void print_test(){
  printf("%d\n",grp_num);
  for(int i=0;i<grp_num;i++){
    for(int j=0;j<grp[i].key_num;j++){
      for(int k=0;k<grp[i].key[j].word_num;k++){
        printf("i=%3d j=%3d k=%3d   ",i,j,k);
        puts(grp[i].key[j].word[k].txt);
      }printf("allcrt=%d\n",grp[i].key[j].allcrt);
    }for(int j=0;j<grp[i].ans_num;j++){
      for(int k=0;k<grp[i].ans[j].word_num;k++){
        printf("i=%3d j=%3d k=%3d   ",i,j,k);
        puts(grp[i].ans[j].word[k].txt);
      }
    }
  }
}
struct thr_data{
  Msg_type tp;
  string txt;
  thr_data(){}
}dt[THR_NUM];
//线程创建时直接传入结构体发现数据不能完整传入故定义全局
int thrr[THR_NUM];
void Thread(thr_data dta){
  srand(time(NULL)+clock()+thr_id);
  Msg_type type=dta.tp;
  for(int i=0;i<grp_num;i++){
    if(grp[i].crt(type,dta.txt.c_str())){
      grp[i].print(type);
      return;
    }
  }if(!type.ifgrp){
    for(int i=0;i<grp_num;i++){
      if(grp[i].crt(type,"[default]")){
        grp[i].print(type);
        return;
      }
    }
  }
  return;
}
thread th1[128];
void Conv(){
  char txxt[1<<15]={0};
  memset(txxt,0,sizeof txxt);
  Msg_type type;
  type=init(txxt);
  if(type.grp_id[0]=='-')goto ed;
  dt[thr_id].tp=type;
  dt[thr_id].txt=txxt;
  thrr[thr_id]=thr_id;
  th1[thr_id]=thread(Thread,dt[thr_id]);
  th1[thr_id++].detach();
  if(thr_id>>7)thr_id=0;
  ed:Sleep(sleep_time);
}
void Heart_Beat(){
  for(;;){
    WinExec(SYMBOL_NAME.c_str(),SW_HIDE);
    Sleep(10000);
  }
}
int main(){
  system("chcp 65001");
  read();
  char tmp[1<<12]={0};
  getcwd(tmp,sizeof tmp);
  RUN_PATH=tmp;
  thread HB(Heart_Beat);
  HB.detach();
  thread DL(Daily);
  DL.detach();
  start_time++;
  read_white_list();
  freopen("CON","r",stdin);
  printf("当前词库信息如下：\n");
  print_test();
  for(;keep_run;){
    Conv();
  }
}
