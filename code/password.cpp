#include<stdio.h>
#include<time.h>
#include<string>
#include<string.h>
#include<iostream>
#include"opstring.h"
using namespace std;


struct Encrypt{

  int tm[5];

  int id[15];
  int n_id;
  string sids;
  string txxt;
  string sid2;
  int pswd;
  int tx_n[1<<11];
  int dx[1<<12];
  Encrypt(){
    memset(tm,0,sizeof tm);
    memset(id,0,sizeof id);
    memset(tx_n,0,sizeof tx_n);
    memset(dx,0,sizeof dx);
    n_id=0;
  }
  void get_id(string sid,string id2){
    long long r=1,r2=1;
    //printf("get_id,%s,%s",sid.c_str(),id2.c_str());
    for(int i=0;i<sid.length();i++){
      int p=sid[i]-'0';
      r*=(p==0?10:p);
    }
    for(int i=0;i<id2.length();i++){
      int p=id2[i]-'0';
      r2*=p;
    }r+=r2;
    r+=pswd;
    char rr[20]={0};
    sprintf(rr,"%lld",r);
    n_id=strlen(rr);
    for(int i=0;i<n_id;i++){
      id[i]=rr[i]-'0';
    }
  }
  void get_tm(){
    int hr,mn;
    time_t tmpcal_ptr;
    struct tm *tmp_ptr = NULL;
    time(&tmpcal_ptr);
    tmp_ptr = localtime(&tmpcal_ptr);
    hr=tmp_ptr->tm_hour;
    mn=tmp_ptr->tm_min;
    char ss[10];
    sprintf(ss,"%02d%02d",hr,mn);
    for(int i=0;i<strlen(ss);i++){
      tm[i]=ss[i]-'0';
    }
  }

  string get_tx(){
    int it_t=0;
    int it_s=0;

    for(int i=0;i<txxt.length();i++){
      tx_n[i]=int(txxt[i])+128;
      printf("%d ",tx_n[i]);
      tx_n[i]+=tm[it_t++];
      tx_n[i]+=tm[it_s++];
      it_t%=4;
      it_s%=n_id;
    }printf("\n");
    string rt="";
    for(int i=0;i<txxt.length();i++){
      dx[(i<<1)]=tx_n[i]/26+65;
      dx[(i<<1)|1]=tx_n[i]%26+65;
      rt=rt+char(dx[i<<1])+char(dx[(i<<1)|1]);
    }
    return rt;
  }
  void init(){

    //puts(txt);
    //puts(txxt.c_str());
    printf("Enter the key of bot: ");
    getline(cin,sids);
    printf("Enter the key of user: ");
    getline(cin,sid2);
    printf("Enter the password: ");
    cin>>pswd;
  }
  void oper(){
    //init();
    printf("Enter the text: ");
    char txt[1<<11];
    gets(txt);
    txxt=".order"+GBKToUTF8(txt);
    get_id(sids,sid2);
    get_tm();
    string s;
    s=get_tx();
    cout<<s<<endl;
    en_txxt=s;
    //de_tx();
  }
  string en_txxt;

  void de_init(){
    getline(cin,en_txxt);
    getline(cin,sids);
    getline(cin,sid2);
  }
  string de_tx(){
    int it_t=0;
    int it_s=0;
    for(int i=0;i<en_txxt.length();i++){
      dx[i]=en_txxt[i]-65;
    }
    for(int i=0;i<(en_txxt.length()>>1);i++){
      tx_n[i]=dx[(i<<1)]*26;
      tx_n[i]+=dx[(i<<1)|1];
      tx_n[i]-=tm[it_t++];
      tx_n[i]-=tm[it_s++]+128;
      it_t%=4;
      it_s%=n_id;
      //putchar(tx_n[i]);
    }
    //printf("\n");
    return "";
  }

  void de_op(){
    de_init();
    get_id(sids,sid2);
    get_tm();
    string s;
    de_tx();
  }
};
int main(){
  //system("chcp 65001");
  Encrypt s;
  s.init();
  for(;;){

    s.oper();
    //s.de_op();
    printf("\n");
  }

}
