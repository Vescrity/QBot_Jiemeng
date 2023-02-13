#ifndef MSG_TYPE
  #define MSG_TYPE
//#include"Msg_get.h"
#include<string>
#include"Jiemeng_Basic.h"
#include"Encrypt.h"
#ifndef IP_ADD
  #define IP_ADD "http://127.0.0.1:"+string(pt)+"/"
#endif
using namespace std;
/*void report_print(const char*);*/

struct Msg_type{
  bool ifgrp;//1-Group msg  0-Private msg
  char grp_id[12];
  char sender_id[14];
  char target_id[14];
  string sender_name;
  string grp_name;
  string Self_ID;
  int ord_lv;
  int msg_id;
  bool if_ignore;
  string msgs;
  Msg_type(){
    ifgrp=0;
    ord_lv=0;
    if_ignore=0;
    memset(grp_id,0,sizeof grp_id);
    memset(sender_id,0,sizeof sender_id);
  }
  void get_grp_name();
  void show(){
    if(ifgrp){
      info_lable("[group_id]");
      info_puts(grp_id);
      info_lable("[group_name]");
      info_puts(grp_name.c_str());
    }
    info_lable("[sender_id]");
    info_puts(sender_id);
    info_lable("[sender_name]");
    info_puts(sender_name.c_str());
    msg_lable("[message]");
    msg_puts(msgs.c_str());
    //if(ifgrp)debug_puts("grp");
    //if(if_ignore)debug_puts("ignore");
  }
  void get_msg_type(const char *txxt){
    //printf("getmsg\n");
    if(strlen(txxt)==0)return;
    int l,r,t,r1,r2;
    string sub_type=json_read(txxt,"sub_type");
    Self_ID=json_read(txxt,"self_id");
    string post_type=json_read(txxt,"post_type");
    if(post_type=="notice"){

      string temps;
      temps=json_read(txxt,"notice_type");

      strcpy(sender_id,json_read(txxt,"user_id").c_str());
      strcpy(grp_id,json_read(txxt,"group_id").c_str());

      ifgrp=(string(grp_id)!="[NONE]");

      if(temps=="notify"){
        temps=json_read(txxt,"sub_type");
        if(temps=="poke"){
          string target=json_read(txxt,"target_id");
          if(target==Self_ID)temps="poke";
          else temps="poke_others";
        }
      }
      msg_id=0;

      msgs="["+temps+"]";
      string cutl="\n-------------------";
      cutl=cutl+CURRENT_VERSION+"-------------------";
      color_puts(cutl.c_str());
      show();
      return;

    }


    if(sub_type!="poke"&&sub_type!="normal"&&sub_type!="friend"&&sub_type!="group"&&sub_type!="anonymous"){
      if_ignore=1;
      info_lable("[INFO]");
      info_puts(txxt);
      return;
    }
    msgs=json_read(txxt,"message");
    string s_id=json_read(txxt,"message_id");
    if(s_id=="[NONE]")msg_id=0;
    else sscanf(s_id.c_str(),"%d",&msg_id);

    msgs=decode(msgs);
    strcpy(sender_id,json_read(txxt,"user_id").c_str());
    strcpy(grp_id,json_read(txxt,"group_id").c_str());
    t=get_st(txxt,"message_type");
    if(t==-1){
      if((l=get_st(txxt,"\"group_id"))!=-1){
        ifgrp=1;
      }goto msg_type_ed;
    }
    t+=15;
    //printf("txxt[t]=%c\n",txxt[t]);
    if(txxt[t]=='g'){
      ifgrp=1;
      sender_name=json_read(txxt,"card");
      if(sender_name.length()==0){
        sender_name=json_read(txxt,"nickname");
      }
    }else {
      strcpy(target_id,json_read(txxt,"target_id").c_str());
      sender_name=json_read(txxt,"nickname");
    }
    if(ifgrp){
      get_grp_name();
    }




    msg_type_ed:
    #ifdef ENCRYPT
      string tmpss=key_repost(Self_ID,sender_id,ENCRYPT_PASSWORD);

      if(get_st(msgs.c_str(),tmpss.c_str())==0){
        msgs=de_cr(Self_ID,sender_id,msgs,ENCRYPT_PASSWORD);
        ord_lv=100;

      }
    #endif
    string cutl="\n-------------------";
    cutl=cutl+CURRENT_VERSION+"-------------------";
    if(sub_type=="poke")msgs="[POKE]";
    color_puts(cutl.c_str());
    show();
  }
};
#endif
