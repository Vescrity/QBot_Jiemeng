#ifndef OPSTRING
  #define OPSTRING
  /*一些常用的字符串操作函数，纯暴力算法*/
#include<string.h>
#include<stdio.h>
bool pfct(const char *a,const char *b){                 //a是否为b的子串
  for(int i=0;i<strlen(b);i++){
    if(*(a+i)!=*(b+i))return 0;
  }return 1;
}
int get_st(const char *a,const char *b){                //获得a在b中的开始位置，如未出现a则-1
  for(int i=0;i<strlen(a);i++){
    if(pfct(a+i,b))return i;
  }return -1;
}
/**********区间字符串复制**********/
inline void get_copy(int l,int r,const char *txt,char *copy){
  for(int i=l;i<r;i++)
    *(copy+i-l)=*(txt+i);
}
inline void strchg(const char *s,const char *str,char *txt){//区间字符串替换
  bool flg=1;
  int l,r,cnt=0;
  char tmp[10000];
  while(flg){flg=0;
    memset(tmp,0,sizeof tmp);
    cnt=0;
    if((l=get_st(txt,s))!=-1){
      flg=1;
      r=l+strlen(s);
      for(int i=0;i<l;i++){
        tmp[cnt++]=txt[i];
      }for(int i=0;i<strlen(str);i++){
        tmp[cnt++]=str[i];
      }for(int i=r;i<strlen(txt);i++){
        tmp[cnt++]=txt[i];
      }for(int i=0;i<=strlen(tmp);i++)txt[i]=tmp[i];
    }//puts(tmp);
  }//puts(txt);
}
#endif
