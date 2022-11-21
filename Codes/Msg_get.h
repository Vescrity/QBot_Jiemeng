#ifndef MSG_GET
  #define MSG_GET
#include"opstring.h"
#include<stdio.h>
void get_sender_id(const char *msg,char *id){
  int l,r1,r2,r;
  l=get_st(msg,"\"user_id\":");
  l+=10;
  r1=get_st(msg+l,",\"");
  r2=get_st(msg+l,"}");
  r=l+(((r1<r2)&&(r1!=-1))?r1:(r2==-1?r1:r2));
  get_copy(l,r,msg,id);
}
void get_msg(char *txt,char *msg){
  //printf("msg=%s\n",msg);
  //printf("sizeof msg=%d\n",sizeof msg);
  memset(msg,0,sizeof msg);
  //printf("msg=%s\n",msg);
  int l,r1,r2,r;
  //l=0xbad;r=0xbad;
  l=get_st(txt,"\"message\":\"");
  if(l==-1){*msg=0;return;}
  r1=get_st(txt+l+11,"\",\"");
  r2=get_st(txt+l+11,"\"}");
  //printf("r1=%d,r2=%d\n",r1,r2);
  l+=11;
  r=l+(((r1<r2)&&(r1!=-1))?r1:(r2==-1?r1:r2));
  get_copy(l,r,txt,msg);
  msg[r-l]=0;
  //puts(msg);
  //for(int i=0;i<strlen(msg);i++)printf("%d ",msg[i]);
  printf("\n");
}
#endif
