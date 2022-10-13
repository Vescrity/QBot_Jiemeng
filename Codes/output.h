#include"Jie_Meng_Base.h"
#ifndef OUTPUT
  #define OUTPUT
  /***消息输出功能集中库***/
#include"Msg_type.h"
#include"File_Read.h"
#include<string>
#include<string.h>
#include<windows.h>
/***指令内容URL编码***/
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
void resend(Msg_type type,const char*msg);                  //等效收到消息
void poke(Msg_type);                                        //戳一戳
string message_chg2(Msg_type type,const char *msg);         //第二次消息内容转换处理
string message_chg(Msg_type type,const char *msg);          //第一次消息内容转换处理
void output(Msg_type type,const char *msg){                 //消息发送
  char pt[6];
  //printf("%d\n",OUTPORT);
  sprintf(pt,"%d",OUTPORT);
  string sid(type.sender_id);
  if(sid==Self_ID)return;
  string opt("curl \"");
  string smsg=message_chg(type,msg);
  if(smsg=="[on]"){main_switch=1;return;}                   //开启总开关
  if(smsg=="[STOP]")return;                                 //停止动作
  if(smsg=="[poke]"){poke(type);return;}                    //发送戳一戳
  if(smsg=="[off]"){main_switch=0;return;}                  //关闭总开关
  if(smsg=="[Restart]"){keep_run=0;return;}                 //关闭程序，在利用start时可实现重启功能
  if(smsg=="[refresh]"){                                    //重新加载词库  p.s.存在未知原因的bug
    grp.clear();
    grp_num=all_ans_num=0;
    read_ans();
    output(type,"已重新加载词库");
    return;
  }
  if(!main_switch||(!keep_run))return;                                  //检查总开关状态
  if(smsg=="[Repeat]")smsg=Current_Msg;                                 //复读所收消息
  if(smsg=="[CQ:image,file=]"){resend(type,"[NO image]");return;}       //空图片消息
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
  printf("\n发送指令：");
  puts(opt.c_str());
  system(opt.c_str());
  msg_send_time++;
}
void poke(Msg_type type){
  string msg("[CQ:poke,qq=");
  string id(type.sender_id);

  msg=msg+id;
  msg=msg+"]";
  output(type,msg.c_str());
}
void Keys::print(Msg_type type){                                                //触发关键词发送对应回复
  for(int i=0;i<this->word_num;i++)
    output(type,this->word[i].txt);
}
void Group::print(Msg_type type){                                               //触发关键词组执行并发送对应回复
    int r=rand()%this->ans_num;
    this->ans[r].print(type);
    printf("\n冷却中(%dms)...\n",sleep_time);
    Sleep(sleep_time);
    printf("冷却结束\n");
}
string message_chg(Msg_type type,const char *msg){
  int l,r,it;
  string rt;
  bool flg=0;
  char ff[2000];
  memset(ff,0,sizeof ff);
  it=0;
  l=get_st(msg,"[name]");
  r=l+6;
  rt=msg;
  if(l!=-1){flg=1;
    it=0;
    for(int i=0;i<l;i++)
      ff[it++]=msg[i];
    //printf("strlen=%d\n",strlen(type.sender_name));
    for(int i=0;i<strlen(type.sender_name);i++)
      ff[it++]=type.sender_name[i];
    for(int i=r;i<strlen(msg);i++)
      ff[it++]=msg[i];
    rt=ff;
    //return rt;
  }
  else{
    l=get_st(msg,"[emoji_草]");                          //empji表情【草】的转换处理
    if(l!=-1){flg=1;
      char tmp[5];
      r=l+10;
      memset(tmp,0,sizeof tmp);
      tmp[0]=-16;
      tmp[1]=-97;
      tmp[2]=-116;
      tmp[3]=-65;
      tmp[4]=0;
      it=0;
      for(int i=0;i<l-6;i++)
        ff[it++]=msg[i];
      for(int i=0;i<strlen(tmp);i++)
        ff[it++]=tmp[i];
      for(int i=r+1;i<strlen(msg);i++)
        ff[it++]=msg[i];
      rt=ff;
    }
    l=get_st(msg,"[rand:");                              //随机数
    if(l!=-1){flg=1;
      char tmp[5];
      memset(tmp,0,sizeof tmp);
      int rd;
      l+=6;
      r=l+get_st(msg+l,"]");
      get_copy(l,r,msg,tmp);
      sscanf(tmp,"%d",&rd);
      memset(tmp,0,sizeof tmp);
      sprintf(tmp,"%d",rand()%rd);
      it=0;
      for(int i=0;i<l-6;i++)
        ff[it++]=msg[i];
      for(int i=0;i<strlen(tmp);i++)
        ff[it++]=tmp[i];
      for(int i=r+1;i<strlen(msg);i++)
        ff[it++]=msg[i];
      rt=ff;
      //return rt;
    }l=get_st(msg,"[rand2:");                            //含范围随机数
    if(l!=-1){flg=1;
      char tmp[10];
      memset(tmp,0,sizeof tmp);
      int rd,base,ll;
      ll=l;
      l+=7;
      r=l+get_st(msg+l,",");
      get_copy(l,r,msg,tmp);
      sscanf(tmp,"%d",&rd);
      l=r+1;
      r=l+get_st(msg+l,"]");
      memset(tmp,0,sizeof tmp);
      get_copy(l,r,msg,tmp);
      sscanf(tmp,"%d",&base);
      memset(tmp,0,sizeof tmp);
      sprintf(tmp,"%d",rand()%rd+base);
      it=0;
      for(int i=0;i<ll;i++)
        ff[it++]=msg[i];
      for(int i=0;i<strlen(tmp);i++)
        ff[it++]=tmp[i];
      for(int i=r+1;i<strlen(msg);i++)
        ff[it++]=msg[i];
      rt=ff;
      //return rt;
    }l=get_st(msg,"[qid]");                              //替换为对方qq号
    if(l!=-1){flg=1;
      char tmp[15];
      memset(tmp,0,sizeof tmp);
      r=l+5;
      //get_copy(l,r,msg,tmp);
      //sscanf(tmp,"%d",&rd);
      memset(tmp,0,sizeof tmp);
      sprintf(tmp,"%s",type.sender_id);
      it=0;
      for(int i=0;i<l;i++)
        ff[it++]=msg[i];
      for(int i=0;i<strlen(tmp);i++)
        ff[it++]=tmp[i];
      for(int i=r;i<strlen(msg);i++)
        ff[it++]=msg[i];
      rt=ff;
    }
    l=get_st(msg,"[163:");                               //网易云音乐分享
    if(l==0){flg=1;
      char tmp[20];
      char ttp[100];
      memset(tmp,0,sizeof tmp);
      l+=5;
      r=l+get_st(msg+l,",");
      get_copy(l,r,msg,tmp);
      l=r+1;
      r=l+get_st(msg+l,"]");
      get_copy(l,r,msg,ttp);
      //sscanf(tmp,"%d",&id);
      tmp[strlen(tmp)]=0;
      sprintf(ff,"[CQ:music,type=163,id=%s]",tmp);
      //sprintf(ff,"[CQ:music,type=custom,url=http://music.163.com/song?id=%d,audio=http://music.163.com/song/media/outer/url?id=%d.mp3,title=%s]",id,id,ttp);
      rt=ff;
      return rt;
    }
    l=get_st(msg,"[resend:");                             //等效收到内容
    if(l==0){flg=1;
      l=8;
      r=strlen(msg)-1;
      get_copy(l,r,msg,ff);
      resend(type,ff);
      return "[STOP]";
    }URLCHG("[\\n]","\n")                                 //换行符替换
  }
  if(flg)return message_chg(type,rt.c_str());             //进行过替换则继续进行下一次替换
  return rt;
}
/***URL编码***/
static unsigned char enc_tab[]="0123456789ABCDEF";
char*acl_url_encode(const char*str){
  int len=(int)strlen(str);
  int tmp_len=len;
  unsigned char*tmp=(unsigned char*)malloc(len+1);
  int i,j;
  for(i=0,j=0;i<len;i++,j++){
    tmp[j]=(unsigned char)str[i];
    if(!isalnum(tmp[j])&&strchr("_-.",tmp[j])==NULL){
      tmp_len+=3;
      tmp=(unsigned char*)realloc(tmp,tmp_len);
      tmp[j++]='%';
      tmp[j++]=enc_tab[(unsigned char)str[i]>>4];
      tmp[j]=enc_tab[(unsigned char)str[i]&0x0F];
    }
  }tmp[j]='\0';
  return (char*)tmp;
}
string message_chg2(Msg_type type,const char*msg){
  printf("\n准备执行输出...\n转换前内容：INPUT=%s\n",msg);
  char*ff=acl_url_encode(msg);
  string rt(ff);
  free(ff);
  printf("转换后内容：OUTPUT=\n%s\n",rt.c_str());
  return rt;
}
/***【老婆生成器】功能的文件读入***/
#define MOE_READ(FILENAME,NAME,NAME_NUM) {\
  freopen(FILENAME,"r",stdin);\
  char tmp[100];\
  tmp[0]=0;\
  gets(tmp);\
  puts(tmp);\
  while(tmp[0]!='/'){\
    sscanf(tmp,"%s",NAME[NAME_NUM++]);\
    gets(tmp);\
  }\
}
/***【老婆生成器】功能的MBTI生成***/
string rand_mbti(){
  char ti[5];
  ti[0]=rand()&1?'I':'E';
  ti[1]=rand()&1?'N':'S';
  ti[2]=rand()&1?'F':'T';
  ti[3]=rand()&1?'P':'J';
  ti[4]=0;
  string rt(ti);
  return rt;
}
#endif
