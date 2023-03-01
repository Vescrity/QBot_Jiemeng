#ifndef MSG_TYPE
  #define MSG_TYPE
//#include"Msg_get.h"
#include<string>
#include"Jiemeng_Basic.h"
#include"Encrypt.h"
#include"json.hpp"
#ifndef IP_ADD
  #define IP_ADD "http://127.0.0.1:"+string(pt)+"/"
#endif
using namespace std;
using json=nlohmann::json;
/*void report_print(const char*);*/
string num2str(long long s){
  char*slfid=new char[20];
  sprintf(slfid,"%lld",(s));
  return slfid;
}

struct Msg_type{
  bool ifgrp;//1-Group msg  0-Private msg
  string grp_id;
  string sender_id;
  string target_id;
  string sender_name;
  string grp_name;
  string Self_ID;
  string role;
  int ord_lv;
  int msg_id;
  bool if_ignore;
  string msgs;
  Msg_type(){
    ifgrp=0;
    ord_lv=0;
    if_ignore=0;

  }
  void get_grp_name();
  void get_sender_name();
  void show(){
    if(ifgrp){
      info_lable("[group_id]");
      info_puts(grp_id.c_str());
      info_lable("[group_name]");
      info_puts(grp_name.c_str());
    }
    info_lable("[sender_id]");
    info_puts(sender_id.c_str());
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
    debug_puts(txxt);

    int l=get_st(txxt,"Content-Length");
    int js_len;
    sscanf(txxt+l,"Content-Length: %d",&js_len);
    json Event;
    Event=json::parse(txxt+(strlen(txxt)-js_len));
    /*char*slfid=new char[20];
    sprintf(slfid,"%lld",(long long)(Event["self_id"]));
    Self_ID=slfid;
    delete slfid;*/
    /*Self_ID=string(Event["self_id"]);
    printf("ok!");*/
    Self_ID=num2str(Event["self_id"]);
    ifgrp=(!Event["group_id"].empty());
    if(Event["user_id"].empty()){
      sender_id="10101";
    }else{
      sender_id=num2str(Event["user_id"]);
    }
    if(ifgrp){
      grp_id=num2str(Event["group_id"]);
      get_grp_name();
    }
    if(Event["post_type"]=="message"||Event["post_type"]=="message_sent"){

      ifgrp=(Event["message_type"]=="group");
      sender_id=num2str(Event["user_id"]);
      msgs=Event["raw_message"];
      sender_name=Event["sender"]["nickname"];
      msg_id=Event["message_id"];
      if(ifgrp){
        if(Event["sub_type"]=="anonymous"){
          sender_name=Event["anonymous"]["name"];
        }else{
          sender_name=Event["sender"]["card"];
          if(sender_name.length()==0)sender_name=Event["sender"]["nickname"];
        }
        role=Event["sender"]["role"];
        grp_id=num2str(Event["group_id"]);
      }


    }
    else if(Event["post_type"]=="notice"){
      msgs="";
      msgs=msgs+"["+string(Event["notice_type"])+"]";
      if(msgs=="[group_decrease]"||msgs=="[group_admin]"||msgs=="[group_ban]"||msgs=="[essence]"){
        msgs=string("[")+string(Event["notice_type"])+"_"+string(Event["sub_type"])+"]";
      }
      if(msgs=="[notify]"){
        msgs=string("[")+string(Event["sub_type"])+"]";
        if(msgs=="[honor]"){
          msgs=string("[")+string(Event["sub_type"])+"_"+string(Event["honor_type"])+"]";
        }
      }
      if(msgs=="[poke]"){
        if(num2str(Event["target_id"])!=Self_ID)msgs="[poke_others]";
      }
    }
    else{
      if_ignore=1;
      info_lable("[INFO]");
      info_puts(txxt+(strlen(txxt)-js_len));
      return;
    }
    /*int l,r,t,r1,r2;
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
    }*/




    if((sender_name.length()==0)&&(sender_id!="10101"||sender_id!="ADMIN"))get_sender_name();
    #ifdef ENCRYPT
      string tmpss=key_repost(Self_ID,sender_id,ENCRYPT_PASSWORD);

      if(get_st(msgs.c_str(),tmpss.c_str())==0){
        msgs=de_cr(Self_ID,sender_id,msgs,ENCRYPT_PASSWORD);
        ord_lv=100;

      }
    #endif
    string cutl="\n-------------------";
    cutl=cutl+CURRENT_VERSION+"-------------------";
    //if(sub_type=="poke")msgs="[POKE]";
    color_puts(cutl.c_str());
    show();
  }
};
#endif
