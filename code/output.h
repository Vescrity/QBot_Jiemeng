#include"Jie_Meng_Base.h"
#ifndef OUTPUT
  #define OUTPUT
#include"Msg_type.h"
#include"File_Read.h"
#include<string>
#include<string.h>
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
  printf("caches[%d]=%s\n",id,caches[id].c_str());
  execmd(caches[id].c_str(),NULL);
}
void run_output_cmd(string msg){
  char bf[1<<9]={0};
  execmd(msg.c_str(),bf);
  string status=json_read(bf,"status");

  if(status=="ok"){
    info_lable("[output]");
    info_puts("Send ok!");
    string msg_id=json_read(bf,"message_id");
    Bot_Judge.push(msg_id);
  }else{
    string word=json_read(bf,"wording");
    error_lable("[output]");
    error_puts(word.c_str());
  }

}
string message_chg2(const Msg_type&type,const char *msg);
string message_chg(Msg_type type,const char *msg);
thread th_opt[128];
int thopid;
void output(Msg_type type,const char *msg){
    //color_print("[output][try]: ");
    //color_puts(msg);
    if(strlen(msg)>(1<<13)){output(type,"消息超长");return;}
    if(strlen(msg)>(1<<11)){
      for(int i=0;(i<<11)<=int(strlen(msg));i++){
        char opts[1<<12]={0};
        get_copy(i<<11,((i+1)<<11)>strlen(msg)?strlen(msg):((i+1)<<11),msg,opts);
        output(type,opts);
        uni_sl(delay_time);
      }return;
    }
    /**指定位置分条发送处理**/
    int lcut,rcut;
    if((lcut=get_st(msg,"[-cut-]"))!=-1){
      char s_msg[1<<11]={0};
      get_copy(0,lcut,msg,s_msg);
      rcut=lcut+strlen("[-cut-]");
      output(type,s_msg);
      uni_sl(delay_time);
      output(type,msg+rcut);
      return;
    }
    char pt[6];
    sprintf(pt,"%d",OUTPORT);
    string sid(type.sender_id);
    //if(sid==Self_ID)return;

    string opt("curl -s \"");
    string smsg=msg;
    smsg=message_chg(type,msg);
    if(type.ord_lv>-1){

      if(smsg=="[on]"){main_switch=1;return;}
      if(smsg=="[STOP]")return;
      if(smsg=="[poke]"){poke(type);return;}
      if(smsg=="[off]"){main_switch=0;return;}
      if(smsg=="[Restart]"){keep_run=0;return;}
      if(smsg=="[Reload]"){
        ans_gp.clear();
        read_ans();
        return;
      }
      if(!main_switch||(!keep_run))return;
      if(smsg=="[CQ:image,file=]"){resend(type,"[NO image]");return;}

    }
    color_lable("[output]");
    color_puts(smsg.c_str());
    if(string(type.grp_id)=="WHITE_LIST"){
      for(int i=1;i<white_list_num;i++){
        Msg_type stype=type;
        strcpy(stype.grp_id,Grp_white_list[i].c_str());
        output(stype,smsg.c_str());
        uni_sl(delay_time);
      }
      return;
    }else if(string(type.sender_id)=="BROAD"){
      for(int i=0;i<broad_pri_num;i++){
        Msg_type stype=type;
        strcpy(stype.sender_id,broad_pri[i].c_str());
        output(stype,smsg.c_str());
        uni_sl(delay_time);
      }
      return;
    }


    if(REPORT_ID.length()>5||REPORT_GROUP.length()>5){
      string rep_sid=type.sender_id;
      string rep_gid=type.grp_id;
      if(rep_sid==REPORT_ID||(rep_gid==REPORT_GROUP&&REPORT_GROUP.length()>5))goto out_report;
      else{
        Msg_type report_type;
        get_copy(0,REPORT_ID.length(),REPORT_ID.c_str(),report_type.sender_id);
        get_copy(0,REPORT_GROUP.length(),REPORT_GROUP.c_str(),report_type.grp_id);
        report_type.ifgrp=REPORT_GROUP.length()>5;
        string report_txxt="";
        if(type.ifgrp){
          report_txxt=report_txxt+"群聊："+type.grp_name+"\n";
          report_txxt=report_txxt+"群号："+type.grp_id+"\n";
        }
        report_txxt=report_txxt+"对象："+type.sender_name+"\n";
        report_txxt=report_txxt+"账号："+type.sender_id+"\n";
        report_txxt=report_txxt+"消息："+"\n"+smsg;
        output(report_type,report_txxt.c_str());
        uni_sl(delay_time);
      }
    }
    out_report:
    smsg=message_chg2(type,smsg.c_str());
    if(type.ifgrp){
      string tmp(type.grp_id);
      opt=opt+IP_ADD+"send_group_msg?group_id="+tmp+"&message="+smsg+"\"";
    }
    else {
      if(strlen(type.sender_id)<2)return;
      string tmp(type.sender_id);
      if(tmp==Self_ID&&(get_st(type.msgs.c_str(),"CQ:image")!=-1)){
        return;
      };
      if(tmp==Self_ID){
        tmp=type.target_id;
      }
      opt=opt+IP_ADD+"send_private_msg?user_id="+tmp+"&message="+smsg+"\"";
    }

    if(type.msgs=="[OUTPUT_JOIN]"){
      th_opt[thopid]=thread(run_output_cmd,opt);
      th_opt[thopid++].join();
    }else{
      th_opt[thopid]=thread(run_output_cmd,opt);
      th_opt[thopid++].detach();
    }

    if(thopid>>7)thopid=0;
    msg_send_time++;

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
  if(id==Self_ID)return;
  msg=msg+id;
  msg=msg+"]";
  output(type,msg.c_str());
}
/*void Keys::print(Msg_type type){
  for(int i=0;i<this->word_num;i++){
    output(type,this->word[i].txt);
    uni_sl(50);
  }
}
void Group::print(Msg_type type){
    int r=rand()%this->ans_num;
    this->ans[r].print(type);
}*/
void ban(Msg_type type,int t){
  string cmds="curl -s \"";
  char pt[6];
  sprintf(pt,"%d",OUTPORT);
  char ttm[10]={0};
  sprintf(ttm,"%d",t*60);
  cmds=cmds+IP_ADD+"set_group_ban?group_id="
      +type.grp_id+"&user_id="+type.sender_id
      +"&duration="+ttm+"\"";
  system(cmds.c_str());
}
void kick(Msg_type type){
  string cmds="curl -s \"";
  char pt[6];
  sprintf(pt,"%d",OUTPORT);
  cmds=cmds+IP_ADD+"set_group_kick?group_id="
      +type.grp_id+"&user_id="+type.sender_id
      +"\"";
  system(cmds.c_str());
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
  l=get_st(msgs,"[ban:");\
  if(l==0){flg=1;\
    int tt;\
    sscanf(msgs,"[ban:%d]",&tt);\
    ban(type,tt);\
    return "[STOP]";\
  }\
  l=get_st(msgs,"[KICK]");\
  if(l==0){flg=1;\
    kick(type);\
    return "[STOP]";\
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
    type.ord_lv=100;\
    type.msgs=ff;\
    resend(type,ff);\
    return "[STOP]";\
  }\
}

struct str_tm{
  char year[6];
  char month[6];
  char mday[6];
  char wday[6];
  char yday[6];
  char hour[3];
  char min[3];
  char sec[3];
  str_tm(){
    time_t now=time(0);
    tm* ltm=localtime(&now);
    sprintf(year,"%d",1900+(ltm->tm_year));
    sprintf(month,"%d",1+(ltm->tm_mon));
    sprintf(mday,"%d",ltm->tm_mday);
    sprintf(hour,"%d",ltm->tm_hour);
    sprintf(min,"%d",ltm->tm_min);
    sprintf(sec,"%d",ltm->tm_sec);
    sprintf(wday,"%d",ltm->tm_wday);
    sprintf(yday,"%d",1+(ltm->tm_yday));
  }
};



string message_chg(Msg_type type,const char *msg){


  int l,r,it;
  string rt,crcode;
  //crcode=msg;
  string*s_msg=new string;
  *s_msg=output_msg_chg(type,msg);
  char msgs[1<<15];
  strcpy(msgs,s_msg->c_str());
  delete s_msg;
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
  char pt[6];
  sprintf(pt,"%d",OUTPORT);
  string ipad=IP_ADD;

  strchg("[Repeat]",type.msgs.c_str(),msgs);
  strchg("[name]",type.sender_name.c_str(),msgs);
  strchg("[group_id]",type.grp_id,msgs);
  strchg("[group_name]",type.grp_name.c_str(),msgs);
  strchg("[qid]",type.sender_id,msgs);
  strchg("[IP_AD]",ipad.c_str(),msgs);
  strchg("[\\n]","\n",msgs);
  strchg("[emoji_草]",ttmp,msgs);
  strchg("\\u003e",">",msgs);
  strchg("\\u003c","<",msgs);
  strchg("\\u0026amp;","&",msgs);

  str_tm*stm=new str_tm;
  strchg("[year]",stm->year,msgs);
  strchg("[month]",stm->month,msgs);
  strchg("[yday]",stm->yday,msgs);
  strchg("[mday]",stm->mday,msgs);
  strchg("[wday]",stm->wday,msgs);
  strchg("[hour]",stm->hour,msgs);
  strchg("[min]",stm->min,msgs);
  strchg("[sec]",stm->sec,msgs);

  delete stm;

  if(type.ord_lv>-1){
    l=get_st(msgs,"[sleep:");
    if(l!=-1){
      int sltm;
      sscanf(msgs,"[sleep:%d]",&sltm);
      uni_sl(sltm);
      return "[STOP]";
    }
  }
  l=get_st(msgs,"[MirlKoi]");
  if(l!=-1){
    while(get_st(msgs,"[MirlKoi]")!=-1){
      string CQ=MirlKoi();
      strchg_1("[MirlKoi]",CQ.c_str(),msgs);
    }
  }l=get_st(msgs,"[HITOKOTO]");
  if(l!=-1){
    while(get_st(msgs,"[HITOKOTO]")!=-1){
      string CQ=hitokoto();
      strchg_1("[HITOKOTO]",CQ.c_str(),msgs);
    }
  }
  l=get_st(msgs,"[RUA]");
  if(l!=-1){
    string CQ=RUA(type.sender_id);
    strchg("[RUA]",CQ.c_str(),msgs);
  }l=get_st(msgs,"[DAILY_NEWS]");
  if(l!=-1){
    string CQ=Daily_news();
    strchg("[DAILY_NEWS]",CQ.c_str(),msgs);
  }
  crcode=msgs;
  crcode=chg_Rcode(crcode);
  strcpy(msgs,crcode.c_str());
  //puts(msgs);
  if(type.ord_lv>-1){
    OUT_PUT_IF;
  }
  rt=msgs;
  if(flg)return message_chg(type,rt.c_str());
  return rt;
}
string message_chg2(const Msg_type&type,const char*msg){
  char*ff=acl_url_encode(msg);
  string rt(ff);
  free(ff);
  //printf("转换后内容：OUTPUT=\n%s\n",rt.c_str());
  return rt;
}

#endif
