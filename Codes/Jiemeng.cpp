#include<stdio.h>
#include<string>
#include<time.h>
#include<iostream>
#include<stdlib.h>
#include"Jie_Meng_Base.h"
#include"Msg_type.h"
#include"Special_Judge.h"
#include"Key_Ans.h"
#include"File_Read.h"

#include"Socket_Input.h"
#pragma comment (lib, "ws2_32.lib")
using namespace std;
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
void Conv(){
  char txxt[99999];
  memset(txxt,0,sizeof txxt);
  Msg_type type;
  type=init(txxt);
  for(int i=0;i<grp_num;i++){
    if(grp[i].crt(type,txxt)){
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
}
int main(){
  system("chcp 65001");
  srand(time(NULL)+clock());
  read();
  start_time++;
  read_white_list();
  freopen("CON","r",stdin);
  printf("当前词库信息如下：\n");
  print_test();
  for(;keep_run;){
    Conv();
  }
}
