#include"Jie_Meng_Base.h"
#ifndef OUTPUT
  #define OUTPUT
#include"Msg_type.h"
#include"File_Read.h"
#include<string>
#include<string.h>
#include<windows.h>
#include<thread>
#include"output_spjfun.h"
#define URLCHG(ORG,CHG)\
  l=get_st(msg,ORG);\
  if(l!=-1){flg=1;\
    r=l+strlen(ORG);\
    char ttmp[5]=CHG;\
    it=0;\
    for(int i=0;i<l;i++)\
      ff[it++]=msg[i];\
    for(int i=0;i<strlen(CHG);i++)\
      ff[it++]=ttmp[i];\
    for(int i=r;i<strlen(msg);i++)\
      ff[it++]=msg[i];\
    ff[it++]=0;\
    rt=ff;\
  }
using namespace std;
void resend(Msg_type type,const char*msg);
string chg_resend(Msg_type type,const char*msg);
void poke(Msg_type);
string caches[128];
int cach_id;
void sys(int id){
  printf("caches[id]=%s\n",caches[id].c_str());
  execmd(caches[id].c_str(),NULL);
}
void curlrun(string msg){
  //printf("com=%s\n",msg.c_str());
  msg="Comrun.exe "+msg+"\0";
  caches[cach_id]=msg;
  //printf("caches[cach_id]=%s\n",caches[cach_id].c_str());
  system(msg.c_str());
  //athss.detach();
  if(cach_id>>7)cach_id=0;
  return;
}
string message_chg2(Msg_type type,const char *msg);
string message_chg(Msg_type type,const char *msg);
thread th_opt[128];
int thopid;

void output(Msg_type type,const char *msg){
  //try{
    if(strlen(msg)>(1<<11)){
      for(int i=0;i<(strlen(msg)>>11);i++){
        char opts[1<<12]={0};
        get_copy(i<<11,((i+1)<<11)>strlen(msg)?strlen(msg):((i+1)<<11),msg,opts);
        output(type,opts);
        Sleep(1000);
      }return;
    }
    char pt[6];
    //printf("%d\n",OUTPORT);
    sprintf(pt,"%d",OUTPORT);
    string sid(type.sender_id);
    if(sid==Self_ID)return;
    string opt("curl \"");
    string smsg=message_chg(type,msg);
    if(smsg=="[on]"){main_switch=1;return;}
    if(smsg=="[STOP]")return;
    if(smsg=="[poke]"){poke(type);return;}
    if(smsg=="[off]"){main_switch=0;return;}
    if(smsg=="[Restart]"){keep_run=0;return;}
    /*if(smsg=="[refresh]"){
      grp.clear();
      grp_num=all_ans_num=0;
      read_ans();
      output(type,"已重新加载词库");
      return;
    }*/
    if(!main_switch||(!keep_run))return;
    //if(smsg=="[Repeat]")smsg=Current_Msg;
    if(smsg=="[CQ:image,file=]"){resend(type,"[NO image]");return;}
    //if(smsg=="[moe_lp]"){printf("lp\n");lpscq(type);return;}
    smsg=message_chg2(type,smsg.c_str());
    //puts(smsg.c_str());
    if(type.ifgrp){
      string tmp(type.grp_id);
      //if(!ifwhite(type))return;
      opt=opt+IP_ADD+"send_group_msg?group_id="+tmp+"&message="+smsg+"\"";
    }
    else {
      if(strlen(type.sender_id)<2)return;
      string tmp(type.sender_id);
      opt=opt+IP_ADD+"send_private_msg?user_id="+tmp+"&message="+smsg+"\"";
    }
    //printf("\n发送指令：");
    //puts(opt.c_str());
    th_opt[thopid]=thread(curlrun,opt);
    th_opt[thopid++].detach();
    if(thopid>>7)thopid=0;
    msg_send_time++;
  //}
  /*catch(...){
    string opts;
    opts="ERROR_in_output()\n";
    opts=opts+"type.sender_id="+type.sender_id+"\n";
    opts=opts+"type.ifgrp="+(type.ifgrp?"1\n":"0\n");
    opts=opts+"msg="+msg+"\n程序错误，请将上述错误信息上报给开发者。";
    puts(opts.c_str());
  }*/
}
/*void error_report(string errors){
  Msg_type type;
  get_copy(0,8,"12346578",type.sender_id);
  type.ifgrp=0;
  output(type,errors.c_str());
}*/
void poke(Msg_type type){
  string msg("[CQ:poke,qq=");
  string id(type.sender_id);
  msg=msg+id;
  msg=msg+"]";
  output(type,msg.c_str());
}
void Keys::print(Msg_type type){
  for(int i=0;i<this->word_num;i++){
    output(type,this->word[i].txt);
    Sleep(50);
  }
}
void Group::print(Msg_type type){
    int r=rand()%this->ans_num;
    this->ans[r].print(type);
}

#define OUT_PUT_IF {\
  l=get_st(msgs,"[rand:");\
  if(l!=-1){flg=1;\
    char tmp[5];\
    memset(tmp,0,sizeof tmp);\
    int rd;\
    l+=6;\
    r=l+get_st(msgs+l,"]");\
    get_copy(l,r,msgs,tmp);\
    sscanf(tmp,"%d",&rd);\
    memset(tmp,0,sizeof tmp);\
    sprintf(tmp,"%d",rand()%rd);\
    it=0;\
    for(int i=0;i<l-6;i++)\
      ff[it++]=msgs[i];\
    for(int i=0;i<strlen(tmp);i++)\
      ff[it++]=tmp[i];\
    for(int i=r+1;i<strlen(msgs);i++)\
      ff[it++]=msgs[i];\
    rt=ff;\
    return message_chg(type,rt.c_str());\
  }l=get_st(msgs,"[rand2:");\
  if(l!=-1){flg=1;\
    char tmp[10];\
    memset(tmp,0,sizeof tmp);\
    int rd,base,ll;\
    ll=l;\
    l+=7;\
    r=l+get_st(msgs+l,",");\
    get_copy(l,r,msgs,tmp);\
    sscanf(tmp,"%d",&rd);\
    l=r+1;\
    r=l+get_st(msgs+l,"]");\
    memset(tmp,0,sizeof tmp);\
    get_copy(l,r,msgs,tmp);\
    sscanf(tmp,"%d",&base);\
    memset(tmp,0,sizeof tmp);\
    sprintf(tmp,"%d",rand()%rd+base);\
    it=0;\
    for(int i=0;i<ll;i++)\
      ff[it++]=msgs[i];\
    for(int i=0;i<strlen(tmp);i++)\
      ff[it++]=tmp[i];\
    for(int i=r+1;i<strlen(msgs);i++)\
      ff[it++]=msgs[i];\
    rt=ff;\
    return message_chg(type,rt.c_str());\
  }\
  l=get_st(msgs,"[163:");\
  if(l==0){flg=1;\
    char tmp[20];\
    char ttp[100];\
    memset(tmp,0,sizeof tmp);\
    l+=5;\
    r=l+get_st(msgs+l,",");\
    get_copy(l,r,msgs,tmp);\
    l=r+1;\
    r=l+get_st(msgs+l,"]");\
    get_copy(l,r,msgs,ttp);\
    tmp[strlen(tmp)]=0;\
    sprintf(ff,"[CQ:music,type=163,id=%s]",tmp);\
    rt=ff;\
    return rt;\
  }\
  l=get_st(msgs,"[resend:");\
  if(l==0){flg=1;\
    l=8;\
    r=strlen(msgs)-1;\
    get_copy(l,r,msgs,ff);\
    resend(type,ff);\
    return "[STOP]";\
  }\
  l=get_st(msgs,"[chg_resend:");\
  if(l!=-1&&flg==0){flg=1;\
    l+=12;\
    r=get_st(msgs,"end_chg]");\
    char ttt[1<<15]={0};\
    get_copy(l-12,r+8,msgs,ttt);\
    get_copy(l,r,msgs,ff);\
    string rr=chg_resend(type,ff);\
    printf("%s\n%s\n%s\n",ttt,rr.c_str(),ff);\
    strchg(ttt,rr.c_str(),msgs);\
    output(type,msgs);\
    return "[STOP]";\
  }\
  l=get_st(msgs,"[chgcrt:");\
  if(l==0&&flg==0){flg=1;\
    l=8;\
    r=strlen(msgs)-1;\
    get_copy(l,r,msgs,ff);\
    Current_Msg=ff;\
    resend(type,ff);\
    return "[STOP]";\
  }\
}
string message_chg(Msg_type type,const char *msg){
  int l,r,it;
  string rt;
  char msgs[1<<15];
  strcpy(msgs,msg);
  bool flg=0;
  char ff[2000];
  char ttmp[5]={0};
  ttmp[0]=-16;
  ttmp[1]=-97;
  ttmp[2]=-116;
  ttmp[3]=-65;
  ttmp[4]=0;
  memset(ff,0,sizeof ff);
  it=0;
  strchg("[Repeat]",Current_Msg.c_str(),msgs);
  strchg("[name]",type.sender_name,msgs);
  strchg("[qid]",type.sender_id,msgs);
  strchg("[\\n]","\n",msgs);
  strchg("[emoji_草]",ttmp,msgs);
  strchg("\\u003e",">",msgs);
  strchg("\\u003c","<",msgs);
  strchg("\\u0026amp;","&",msgs);

  l=get_st(msgs,"[sleep:");
  if(l!=-1){
    int sltm;
    sscanf(msgs,"[sleep:%d]",&sltm);
    Sleep(sltm);
    return "[STOP]";
  }l=get_st(msgs,"[MirlKoi]");
  if(l!=-1){
    while(get_st(msgs,"[MirlKoi]")!=-1){
      string CQ=MirlKoi();
      strchg_1("[MirlKoi]",CQ.c_str(),msgs);
    }
  }
  l=get_st(msgs,"[RUA]");
  if(l!=-1){
    string CQ=RUA(type.sender_id);
    strchg("[RUA]",CQ.c_str(),msgs);
  }l=get_st(msgs,"[HITOKOTO]");
  if(l!=-1){
    string CQ=hitokoto();
    strchg("[HITOKOTO]",CQ.c_str(),msgs);
  }l=get_st(msgs,"[DAILY_NEWS]");
  if(l!=-1){
    string CQ=Daily_news();
    strchg("[DAILY_NEWS]",CQ.c_str(),msgs);
  }
  //puts(msgs);
  OUT_PUT_IF;

  rt=msgs;
  if(flg)return message_chg(type,rt.c_str());
  return rt;
}
string message_chg2(Msg_type type,const char*msg){
  //printf("\n执行输出：\n%s\n",msg);
  char*ff=acl_url_encode(msg);
  string rt(ff);
  free(ff);
  //printf("转换后内容：OUTPUT=\n%s\n",rt.c_str());
  return rt;
}

#endif
