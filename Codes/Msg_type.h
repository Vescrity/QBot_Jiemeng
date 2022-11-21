#ifndef MSG_TYPE
  #define MSG_TYPE
#include"Msg_get.h"
#include<string>
#ifndef IP_ADD
  #define IP_ADD "http://127.0.0.1:"+string(pt)+"/"
#endif
using namespace std;
/*void report_print(const char*);*/

struct Msg_type{
  bool ifgrp;//1-Group msg  0-Private msg
  char grp_id[12];
  char sender_id[14];
  char sender_name[100];
  char grp_name[200];
  //char group_name[50];
  Msg_type(){
    ifgrp=0;
    memset(grp_id,0,sizeof grp_id);
    memset(sender_id,0,sizeof sender_id);
    memset(sender_name,0.,sizeof sender_name);
    memset(grp_name,0.,sizeof grp_name);
  }void get_grp_name(char*);
  void get_msg_type(const char *txxt){
    //printf("getmsg\n");
    if(strlen(txxt)==0)return;
    int l,r,t,r1,r2;
    t=get_st(txxt,"message_type");
    if(t==-1){
      if((l=get_st(txxt,"\"group_id"))!=-1){
        ifgrp=1;
        l+=11;
        r1=get_st(txxt+l,",\"");
        r2=get_st(txxt+l,"}");
        r=l+(((r1<r2)&&(r1!=-1))?r1:(r2==-1?r1:r2));
        //r+=l;
        get_copy(l,r,txxt,grp_id);
      }goto msg_type_ed;
    }
    t+=15;
    //printf("txxt[t]=%c\n",txxt[t]);
    if(txxt[t]=='g'){
      l=get_st(txxt,"\"group_id");
      ifgrp=1;
      l+=11;
      r1=get_st(txxt+l,",\"");
      r2=get_st(txxt+l,"}");
      r=l+(((r1<r2)&&(r1!=-1))?r1:(r2==-1?r1:r2));
      //r+=l;
      get_copy(l,r,txxt,grp_id);
      l=get_st(txxt,"\"card\":\"");
      l+=8;
      r=get_st(txxt,"\",\"level\"");
      get_copy(l,r,txxt,sender_name);
      if(strlen(sender_name)==0){
        l=get_st(txxt,"\"nickname\":\"");
        r=get_st(txxt,"\",\"role");
        l+=12;
        get_copy(l,r,txxt,sender_name);
        sender_name[r-l]=0;
      }
    }else {
      ifgrp=0;
      l=get_st(txxt,"\"nickname\":\"");
      r=get_st(txxt,"\",\"sex");
      l+=12;
      get_copy(l,r,txxt,sender_name);
    }
    /*if(ifgrp){
      get_grp_name(grp_name);
    }*/
    msg_type_ed:get_sender_id(txxt,sender_id);
    /*puts("\n---------------------------------------------------");
    printf("\n收到消息\nGroup_ID=%s\n",grp_id);
    printf("Group_name=%s\n",grp_name);
    printf("Sender_ID=%s\n",sender_id);
    printf("Sender_name=%s\n",sender_name);
    printf("Message=");*/
  }
};
#endif
