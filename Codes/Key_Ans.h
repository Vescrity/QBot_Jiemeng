#ifndef KEY_ANS
  #define KEY_ANS
  /*应答组的类型定义库*/
#include<stdlib.h>
#include<vector>
#include<stdio.h>
#include<time.h>
#include"Jie_Meng_Base.h"
#include"Msg_type.h"
using namespace std;
struct Words{                                   //关键词
  char txt[10000];
  bool crt(const char *s){                      //判断是否匹配
    int l=strlen(txt);
    for(int i=0;i<l;i++){
      if(txt[i]!=*(s+i))return 0;
    }return 1;
  }
};
struct Keys{                                    //关键词组
  vector<Words>word;
  bool allcrt;
  int word_num;
  Keys(){word_num=allcrt=0;}
  bool crt(const char *s){                      //是否触发关键词组
    if(allcrt){
      if(strlen(s)==strlen(word[0].txt)){
        return word[0].crt(s);
      }
    }else{
      int n=strlen(s);
      bool flg;
      if(word_num==0)return 1;
      for(int i=0;i<word_num;i++){
        flg=0;
        int m=strlen(word[i].txt);
        for(int j=0;j<=n-m;j++){
          if(word[i].crt(s+j))flg=1;
        }if(!flg)return 0;
      }return 1;
    }return 0;
  }
  void insert_word(Words a){                     //插入新的关键词
    word.push_back(a);
    word_num++;
  }
  void print(Msg_type);                           //执行输出
};
struct Group{                                     //应答组类型定义
  vector<Keys>key;
  vector<Keys>ans;
  int key_num,ans_num,pri;                        //pri:优先级
  bool sp,sg,priv;                                //sp:该应答组是否限定特定人可触发 sg:（群）
  string spid;
  void insert_key(Keys a){key.push_back(a);key_num++;}
  void insert_ans(Keys a){ans.push_back(a);ans_num++;}
  bool crt(Msg_type type,const char *s){
    if(sp){
      if(spid!=type.sender_id)
        return 0;
    }if(sg){
      if(spid!=type.grp_id)
        return 0;
    }if(priv){
      if(!check_priv(type.sender_id))return 0;
    }//if(key_num==0)return 1;
    for(int i=0;i<key_num;i++){
      if(key[i].crt(s))return 1;
    }return 0;
  }
  void print(Msg_type);
  Group(){priv=sg=sp=key_num=ans_num=pri=0;}
};
bool operator <(Group a,Group b){return a.pri>b.pri;}     //为了按优先级排序
#endif
