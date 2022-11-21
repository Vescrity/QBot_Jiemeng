#ifndef KEY_ANS
  #define KEY_ANS
#include<stdlib.h>
#include<vector>
#include<stdio.h>
#include<time.h>
#include"Jie_Meng_Base.h"
#include"Msg_type.h"
using namespace std;
struct Words{
  char txt[1<<14];
  bool crt(const char *s){
    int l=strlen(txt);
    for(int i=0;i<l;i++){
      if(txt[i]!=*(s+i))return 0;
    }return 1;
  }
};
struct Keys{
  vector<Words>word;
  bool allcrt;
  int word_num;
  Keys(){word_num=allcrt=0;}
  bool crt(const char *s){
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
  void insert_word(Words a){
    word.push_back(a);
    word_num++;
  }
  void print(Msg_type);
};
struct Group{
  vector<Keys>key;
  vector<Keys>ans;
  int key_num,ans_num,pri;
  bool sp,sg,priv;
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
bool operator <(Group a,Group b){return a.pri>b.pri;}
#endif
