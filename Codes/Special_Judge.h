
  #include<string>
  #include"output.h"
  #include"File_Read.h"
  #include"Jie_Meng_Base.h"
#ifndef SPECIAL_JUDGE
  #define SPECIAL_JUDGE
  /**奇葩功能集中库**/
  #include"Game.h"
  #include<map>
using namespace std;
map<string,Connect_Four>game_list;                                              //当前为四子棋游戏状态存储表
void lpscq(Msg_type);                                                           //老婆生成器
bool execmd(const char* cmd,char* result);
/**********Special Send***************/
void send_poke(Msg_type type,char *target_id){                                  //戳一戳发送
  string msg("[CQ:poke,qq=");
  string id(target_id);
  msg=msg+id;
  msg=msg+"]";
  output(type,msg.c_str());
  for(int i=0;i<grp_num;i++){
    if(grp[i].crt(type,"[poke]")){
      grp[i].print(type);
      return;
    }
  }
}
string tags(const char*txt,int l,const char c){                                 //部分图片搜索api的关键词部分url生成
  string rt;
  char s[2];
  rt="\0";
  s[1]=0;
  rt+=c;
  rt+="tag=";
  for(int i=l+1;i<strlen(txt);i++){
    if(txt[i]=='#'){
      rt+="&tag=";continue;
    }if(txt[i]==']')break;
    s[0]=txt[i];
    rt+=acl_url_encode(s);
  }
  rt+="\0";
  return rt;
}
/**********Special Judge****************/
bool spj_poke(const char *msg){                                                 //判断所收消息是否为戳一戳
  string aa("\"target_id\":");
  aa=aa+Self_ID;
  return get_st(msg,"\"sub_type\":\"poke\"")!=-1&&get_st(msg,aa.c_str())!=-1;
}bool spj_rand(const char *msg){                                                //判断消息内容是否含有【.rd】
  return pfct(msg,".rd");
}bool spj_wyy(const char *msg){                                                 //判断消息内容开头是否为【点歌】
  return get_st(msg,"点歌")==0;
}
bool spj_jrrp(const char*msg){                                                  //判断消息内容是否含有【.jrrp】
  return pfct(msg,".jrrp");
}bool spj_pfct(const char*txxt,const char*msg){                                 //判断消息内容是否为指定内容
  return pfct(msg,txxt)&&strlen(msg)==strlen(txxt);
}
bool spj(Msg_type type,const char *buf,const char*msg){                         //是否触发代码内特殊内容
  char temp[100],temp2[10];
  memset(temp,0,sizeof temp);
  memset(temp2,0,sizeof temp2);
  if(spj_poke(buf)){
    get_sender_id(buf,temp);
    if(get_st(temp,Self_ID.c_str())!=-1)return 0;
    //send_poke(type,temp);
    for(int i=0;i<grp_num;i++){
      if(grp[i].crt(type,"[poke]")){
        grp[i].print(type);
        break;
      }
    }
    return 1;
  }if(spj_wyy(msg)){                                                            //网易云点歌
    int l,r;
    l=strlen("点歌");
    r=strlen(msg);
    char name[128],sid[20];
    memset(name,0,sizeof name);
    memset(sid,0,sizeof sid);
    get_copy(l,r,msg,name);
    char*ff=acl_url_encode(name);
    string rt(ff);
    free(ff);
    puts(rt.c_str());
    string ord="curl ";
    string url="\"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
    url=url+rt+"&type=1&offset=0&total=true&limit=1\"";
    puts(url.c_str());
    char bf[2048];
    memset(bf,0,sizeof bf);
    ord=ord+url;
    execmd(ord.c_str(),bf);
    l=get_st(bf,"[{\"id\":");
    r=get_st(bf+l,",\"");
    r+=l;
    get_copy(l+7,r,bf,sid);
    string opt="[163:";
    opt=opt+sid+",qwq]";
    output(type,opt.c_str());
    return 1;
  }if(spj_pfct("[rec_163]",msg)){                                               //发送指定歌曲的语音
    int l,r;
    l=get_st(Current_Msg.c_str(),"#");
    r=strlen(Current_Msg.c_str());
    if(l==-1)return 0;
    char name[128],sid[20];
    memset(name,0,sizeof name);
    memset(sid,0,sizeof sid);
    get_copy(l,r,Current_Msg.c_str(),name);
    char*ff=acl_url_encode(name);
    string rt(ff);
    free(ff);
    puts(rt.c_str());
    string ord="curl ";
    string url="\"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
    url=url+rt+"&type=1&offset=0&total=true&limit=1\"";
    puts(url.c_str());
    char bf[2048];
    memset(bf,0,sizeof bf);
    ord=ord+url;
    execmd(ord.c_str(),bf);
    l=get_st(bf,"[{\"id\":");
    r=get_st(bf+l,",\"");
    r+=l;
    get_copy(l+7,r,bf,sid);
    string opt="[CQ:record,file=http://music.163.com/song/media/outer/url?id=";
    opt=opt+sid+".mp3]";
    output(type,opt.c_str());
    return 1;
  }if(get_st(msg,"[lolicon_setu")==0){                                          //调用lolicon api返回一张图片，并可加关键词
    int l,r;
    char bf[2048];
    memset(bf,0,sizeof bf);
    string str,str2;
    str="curl \"https://api.lolicon.app/setu/v2";
    str2=tags(msg,13,'?');
    str=str+str2+"\"";
    puts(str.c_str());
    execmd(str.c_str(),bf);
    l=get_st(bf,"{\"original\"");
    r=get_st(bf+l,"\"}");
    r+=l;
    l+=13;
    string opt="[CQ:image,file=";
    char furl[512];
    memset(furl,0,sizeof furl);
    get_copy(l,r,bf,furl);
    puts(furl);
    opt=opt+furl+"]";
    output(type,opt.c_str());
    return 1;
  }if(get_st(msg,"[r18_setu")==0){                                              //调用lolicon api返回一张r18图片，并可加关键词
    int l,r;
    char bf[2048];
    string str,str2;
    str="curl \"https://api.lolicon.app/setu/v2";
    str2=tags(msg,9,'&');
    str=str+"?r18=1"+str2+"\"";
    puts(str.c_str());
    memset(bf,0,sizeof bf);
    execmd(str.c_str(),bf);
    puts(bf);
    l=get_st(bf,"{\"original\"");
    r=get_st(bf+l,"\"}");
    r+=l;
    l+=13;
    string opt="[CQ:image,file=";
    char furl[512];
    memset(furl,0,sizeof furl);
    get_copy(l,r,bf,furl);
    puts(furl);
    opt=opt+furl+"]";
    output(type,opt.c_str());
    return 1;
  }if(spj_pfct("[lolicon_sotu]",msg)){                                          //启动搜图功能（lolicon_api）
    int l=get_st(Current_Msg.c_str(),"#");
    if(l==-1){resend(type,"[lolicon_setu]");return 1;}
    int r=strlen(Current_Msg.c_str());
    char str[1024];
    get_copy(l,r,Current_Msg.c_str(),str);
    string a;
    a="[lolicon_setu";
    a+=str;
    a+="]";
    resend(type,a.c_str());
    return 1;
  }if(spj_pfct("[r18_sotu]",msg)){
    int l=get_st(Current_Msg.c_str(),"#");
    if(l==-1){resend(type,"[r18_setu]");return 1;}
    int r=strlen(Current_Msg.c_str());
    char str[1024];
    get_copy(l,r,Current_Msg.c_str(),str);
    string a;
    a="[r18_setu";
    a+=str;
    a+="]";
    resend(type,a.c_str());
    return 1;
  }

  if(spj_pfct("[sakura_setu]",msg)){                                            //调用sakura api返回一张图片
    int l,r;
    char bf[2048];
    memset(bf,0,sizeof bf);
    execmd("curl https://www.dmoe.cc/random.php?return=json",bf);
    l=get_st(bf,"\"imgurl\":\"");
    r=get_st(bf+l+15,"\"");
    r+=l+15;
    l+=10;
    puts(bf);

    string opt="[CQ:image,file=";
    char ffurl[512],furl[512];
    memset(ffurl,0,sizeof ffurl);memset(furl,0,sizeof furl);
    get_copy(l,r,bf,ffurl);
    puts(ffurl);
    int j=0;
    for(int i=0;i<strlen(ffurl);i++)
      if(ffurl[i]!='\\')furl[j++]=ffurl[i];
    puts(furl);
    opt=opt+furl+"]";
    output(type,opt.c_str());
    return 1;
  }if(spj_pfct("[MirlKoi_setu]",msg)){
    int l,r;
    char bf[2048];
    memset(bf,0,sizeof bf);
    execmd("curl \"https://iw233.cn/api.php?sort=random&type=json\"",bf);
    l=get_st(bf,"[\"");
    r=get_st(bf+l+2,"\"");
    r+=l+2;
    l+=2;
    puts(bf);

    string opt="[CQ:image,file=";
    char ffurl[512],furl[512];
    memset(ffurl,0,sizeof ffurl);memset(furl,0,sizeof furl);
    get_copy(l,r,bf,ffurl);
    puts(ffurl);
    int j=0;
    for(int i=0;i<strlen(ffurl);i++)
      if(ffurl[i]!='\\')furl[j++]=ffurl[i];
    puts(furl);
    opt=opt+furl+"]";
    output(type,opt.c_str());
    return 1;
  }if(spj_pfct("[tohou_setu]",msg)){                                            //发送一张东方图片
    int l,r;
    char bf[2048];
    memset(bf,0,sizeof bf);
    execmd("curl https://img.paulzzh.com/touhou/random?type=json",bf);
    l=get_st(bf,"/image/");
    r=get_st(bf+l+8,"/");
    r+=l+8;
    l+=7;
    puts(bf);
    string opt="[CQ:image,file=https://img.paulzzh.com/touhou/konachan/image/";
    char furl[512];
    memset(furl,0,sizeof furl);
    get_copy(l,r,bf,furl);
    puts(furl);
    opt=opt+furl+".jpg]";
    output(type,opt.c_str());
    return 1;
  }
  /*********************计时器********************/
  if(spj_pfct("[tic]",msg)){                                                    //指令：启动计时器
    int l=get_st(Current_Msg.c_str(),"#");
    if(l==-1){output(type,"错误：未指定计时器名称");return 1;}
    int r=strlen(Current_Msg.c_str());
    char str[1024];
    get_copy(l+1,r,Current_Msg.c_str(),str);
    string a;
    a=str;
    time_list[a]=clock();
    output(type,"计时添加成功");
    //resend(type,a.c_str());
    return 1;
  }if(spj_pfct("[toc]",msg)){                                                   //指令：查看计时器
    int l=get_st(Current_Msg.c_str(),"#");
    if(l==-1){output(type,"错误：未指定计时器名称");return 1;}
    int r=strlen(Current_Msg.c_str());
    char str[1024];
    get_copy(l+1,r,Current_Msg.c_str(),str);
    string a;
    a=str;
    if(time_list.find(a)==time_list.end()){
      output(type,"错误：不存在该计时器");return 1;
    }
    int dt=clock()-time_list[a];
    int sec,min,hr;
    dt/=1000;
    sec=dt%60;
    dt/=60;
    min=dt%60;
    dt/=60;
    hr=dt;
    char opt[512];
    memset(opt,0,sizeof opt);
    sprintf(opt,"计时器%s用时：%d:%02d:%02d",a.c_str(),hr,min,sec);
    output(type,opt);
    //resend(type,a.c_str());
    return 1;
  }
  if(spj_pfct("[moe_lp]",msg)){                                                 //调用老婆生成器
    puts("lp");
    lpscq(type);
    return 1;
  }
  if(spj_pfct("[Connect_Four_Start]",msg)){                                     //指令：开始游戏【四子棋】
    int l=get_st(Current_Msg.c_str(),"#");
    if(l==-1){output(type,"错误：未指定棋局名称");return 1;}
    int r=strlen(Current_Msg.c_str());
    char str[1024];
    get_copy(l+1,r,Current_Msg.c_str(),str);
    string a(str);
    Connect_Four aa;
    game_list[a]=aa;
    output(type,game_list[a].opr("start").c_str());
    //resend(type,a.c_str());
    return 1;
  }if(spj_pfct("[Connect_Four]",msg)){                                          //指令：游戏【四子棋】操作
    int l=get_st(Current_Msg.c_str(),"#");
    if(l==-1){output(type,"错误：未指定棋局名称");return 1;}
    int r=get_st(Current_Msg.c_str()+l+1,"#");
    r+=l+1;
    char str[1024];
    get_copy(l+1,r,Current_Msg.c_str(),str);
    string a(str);
    if(game_list.find(a)==game_list.end()){
      output(type,"错误：不存在该棋局");return 1;
    }
    l=r;
    r=strlen(Current_Msg.c_str());
    get_copy(l+1,r,Current_Msg.c_str(),str);
    //string b(str);
    string optt;
    optt="棋局"+a+"\n";
    output(type,(optt+game_list[a].opr(str)).c_str());
    //resend(type,a.c_str());
    return 1;
  }
  if(spj_pfct("[status]",msg)){                                                 //发送统计信息
    char sttmtxt[200];
    memset(sttmtxt,0,sizeof sttmtxt);
    sprintf(sttmtxt,"当前统计信息如下：[\\n]累计启动：%d次[\\n]累计响应消息：%d次[\\n][\\n]当前词库共有：[\\n]应答组：%d组[\\n]回复：%d条",start_time,msg_send_time,grp_num,all_ans_num);
    output(type,sttmtxt);
    memset(sttmtxt,0,sizeof sttmtxt);
    return 1;
  }if(get_st(msg,".resend")==0&&(check_priv(type.sender_id))){                  //resend指令
    char tmp[1000];
    memset(tmp,0,sizeof tmp);
    get_copy(7,strlen(msg),msg,tmp);
    //tmp[strlen(tmp)]=0
    strchg("\\u0026#91;","[",tmp);
    strchg("\\u0026#93;","]",tmp);
    puts(tmp);
    resend(type,tmp);
    return 1;
  }if(get_st(msg,".repost")==0&&(check_priv(type.sender_id))){                  //repost指令
    char tmp[1000];
    memset(tmp,0,sizeof tmp);
    get_copy(7,strlen(msg),msg,tmp);
    //tmp[strlen(tmp)]=0
    strchg("\\u0026#91;","[",tmp);
    strchg("\\u0026#93;","]",tmp);
    puts(tmp);
    output(type,tmp);
    return 1;
  }if(get_st(msg,".type_repost")==0&&(check_priv(type.sender_id))){             //指定目标的repost指令
    char tmp[1000];
    Msg_type ntype;
    memset(tmp,0,sizeof tmp);
    int l,r;
    l=get_st(msg,"#");
    r=get_st(msg+l+1,"#")+l+1;
    get_copy(l+1,r,msg,tmp);
    if(r-l<9)
      ntype.ifgrp=0;
    else ntype.ifgrp=1;
    get_copy(0,strlen(tmp),tmp,ntype.grp_id);
    l=r+1;
    r=get_st(msg+l+1,"#")+l+1;
    memset(tmp,0,sizeof tmp);
    get_copy(l,r,msg,ntype.sender_id);
    get_copy(r+1,strlen(msg),msg,tmp);
    //tmp[strlen(tmp)]=0
    strchg("\\u0026#91;","[",tmp);
    strchg("\\u0026#93;","]",tmp);
    puts(tmp);
    output(ntype,tmp);
    return 1;
  }if(get_st(msg,".type_resend")==0&&(check_priv(type.sender_id))){             //指定目标的resend指令
    char tmp[1000];
    Msg_type ntype;
    memset(tmp,0,sizeof tmp);
    int l,r;
    l=get_st(msg,"#");
    r=get_st(msg+l+1,"#")+l+1;
    get_copy(l+1,r,msg,tmp);
    if(r-l<9)
      ntype.ifgrp=0;
    else ntype.ifgrp=1;
    get_copy(0,strlen(tmp),tmp,ntype.grp_id);
    l=r+1;
    r=get_st(msg+l+1,"#")+l+1;
    memset(tmp,0,sizeof tmp);
    get_copy(l,r,msg,ntype.sender_id);
    get_copy(r+1,strlen(msg),msg,tmp);
    //tmp[strlen(tmp)]=0
    strchg("\\u0026#91;","[",tmp);
    strchg("\\u0026#93;","]",tmp);
    puts(tmp);
    resend(ntype,tmp);
    return 1;
  }if(spj_pfct(".print",msg)&&!type.ifgrp){                                     //词库输出
    freopen(ANS_FILENAME.c_str(),"r",stdin);
    char txt[4000];memset(txt,0,sizeof txt);
    char opt[10000];memset(opt,0,sizeof opt);
    while(txt[0]!='/'){
      memset(opt,0,sizeof opt);
      for(int i=0;i<15&&txt[0]!='/';i++){
        memset(txt,0,sizeof txt);
        gets(txt);
        get_copy(0,strlen(txt),txt,opt+strlen(opt));
        get_copy(0,4,"[\\n]",opt+strlen(opt));
      }
      output(type,opt);
    }
    return 1;
  }
  if(spj_pfct(".info",msg)){                                                    //版本信息
    output(type,"程序版本：v10.7 更新于2022-10-14 18:38[\\n]开发by雪と葉[\\n]项目地址:https://github.com/Vescrity/QBot_Jiemeng[\\n]请勿滥用bot酱呐♥");
    return 1;
  }if(spj_pfct(".switch on",msg)&&(check_priv(type.sender_id))){                //开启应答

    main_switch=1;
    return 1;
  }if(spj_pfct(".switch off",msg)){                                             //关闭应答
    main_switch=0;
    return 1;
  }
  if(spj_rand(msg)){                                                            //生成随机数
    get_copy(3,strlen(msg),msg,temp2);
    int rr;
    sscanf(temp2,"%d",&rr);
    memset(temp2,0,sizeof temp2);
    if(rr==0){
      output(type,"你想让我卡死吗，不要让我除以0啊Kora!");
      return 1;
    }
    sprintf(temp2,"%d",rand()%rr);
    output(type,temp2);
    return 1;
  }//if(spj_pfct("[老婆生成器]",msg))
  return 0;
}
void lpscq(Msg_type type){
    printf("调用lpscq\n");
    char optmsg[2000];
    char zokusai[200][20],kami[50][15],kamiclr[50][15],eye[50][15],eyeclr[50][15];
    int zokusai_num=0,kami_num=0,kamiclr_num=0,eye_num=0,eyeclr_num=0;
    int rrd=rand()%100;
    memset(optmsg,0,sizeof optmsg);
    MOE_READ("moe_zokusai.txt",zokusai,zokusai_num)
    MOE_READ("moe_kami.txt",kami,kami_num)
    MOE_READ("moe_kamiclr.txt",kamiclr,kamiclr_num)
    MOE_READ("moe_eye.txt",eye,eye_num)
    MOE_READ("moe_eyeclr.txt",eyeclr,eyeclr_num)
    sprintf(optmsg,"又来啦？少yy些奇怪的东西吧！[\\n][name]YY出的老婆：[\\n]");
    string mbti=rand_mbti();
    int ht,wt,rsttime=0;
    rst:ht=110+rand()%81;
    wt=20+rand()%60;
    double bmi=wt/(ht*ht/10000.0);
    if(bmi>24.5||bmi<16){
      if(rand()%100>5){
        rsttime++;
        goto rst;
      }
    }int midnum[5];
    if(rand()%100<3){bom:sprintf(optmsg+strlen(optmsg),"[\\n]……哎呀，[name]的老婆爆炸了！");goto moe_opt;}
    sprintf(optmsg+strlen(optmsg),"【性别】：");
    if(rrd<3)sprintf(optmsg+strlen(optmsg),"男");
    else if(rrd<7)sprintf(optmsg+strlen(optmsg),"伪郎");
    else if(rrd<15)sprintf(optmsg+strlen(optmsg),"伪娘");
    else if(rrd<18)sprintf(optmsg+strlen(optmsg),"无性别");
    else if(rrd<20)sprintf(optmsg+strlen(optmsg),"扶她");
    else if(rrd<100)sprintf(optmsg+strlen(optmsg),"女");
    sprintf(optmsg+strlen(optmsg),"[\\n]【身高】：%dcm[\\n]【体重】：%dkg[\\n]",ht,wt);
    sprintf(optmsg+strlen(optmsg),"【CUP】：");
    rrd=rand()%6;
    if(rrd==0)sprintf(optmsg+strlen(optmsg),"A-[\\n]");
    else sprintf(optmsg+strlen(optmsg),"%c[\\n]",'A'+rrd-1);
    if(rrd==5){
      sprintf(optmsg+strlen(optmsg),"[\\n]渡劫判定！！");
      midnum[0]=rand()%20;
      midnum[1]=rand()%100;
      sprintf(optmsg+strlen(optmsg),"[\\n]神秘力量的威力值为%d！！[\\n][name]的老婆的抵抗数值为%d！！[\\n]",midnum[1],midnum[0]);
      if(midnum[0]>midnum[1])sprintf(optmsg+strlen(optmsg),"[\\n]渡⭐劫⭐大⭐成⭐功[\\n]");
      else {sprintf(optmsg+strlen(optmsg),"[\\n]渡⭐劫⭐大⭐失⭐败[\\n]");goto bom;}
    }
    sprintf(optmsg+strlen(optmsg),"【发型】：%s[\\n]",kami[rand()%kami_num]);
    sprintf(optmsg+strlen(optmsg),"【发色】：%s[\\n]",kamiclr[rand()%kamiclr_num]);
    sprintf(optmsg+strlen(optmsg),"【眼型】：%s[\\n]",eye[rand()%eye_num]);
    sprintf(optmsg+strlen(optmsg),"【眼色】：%s[\\n]",eyeclr[rand()%eyeclr_num]);
    sprintf(optmsg+strlen(optmsg),"【萌点】：%s[\\n]",zokusai[rand()%zokusai_num]);
    sprintf(optmsg+strlen(optmsg),"【MBTI】：%s",mbti.c_str());
    sprintf(optmsg+strlen(optmsg),"[\\n][name]，你知道吗，你的老婆在诞生过程中经历了%d次重开哦",rsttime);
    moe_opt:output(type,optmsg);
    return;
}
#endif
