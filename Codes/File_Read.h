#ifndef FILE_READ
  #define FILE_READ
#include<algorithm>
#include"Jie_Meng_Base.h"
void getss(char *s){
  gets(s);
  if(s[0]=='#'||s[0]=='\0')getss(s);
}
void read_stats(){
  info_lable("[File_Read]");
  info_puts("读取统计信息");
  freopen("stats.data","r",stdin);
  char tmptxt[50];
  gets(tmptxt);
  STAT_START_TIME=tmptxt;
  gets(tmptxt);
  sscanf(tmptxt,"start_time=%d",&start_time);
  gets(tmptxt);
  sscanf(tmptxt,"msg_send_time=%d",&msg_send_time);
}
void read_config(){
  read_stats();
  info_lable("[File_Read]");
  info_puts("加载配置文件");
  freopen("config.cfg","r",stdin);
  {
    char tmptxt[100];
    memset(tmptxt,0,sizeof tmptxt);
    gets(tmptxt);
    sscanf(tmptxt,"Self_ID=%s",tmptxt);
    Self_ID=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"REPORT_ID=%s",tmptxt);
    REPORT_ID=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"REPORT_GROUP=%s",tmptxt);
    REPORT_GROUP=tmptxt;

    gets(tmptxt);
    sscanf(tmptxt,"INPORT=%d",&INPORT);
    gets(tmptxt);
    sscanf(tmptxt,"OUTPORT=%d",&OUTPORT);
    gets(tmptxt);
    sscanf(tmptxt,"sleep_time=%d",&sleep_time);
    gets(tmptxt);
    sscanf(tmptxt,"delay_time=%d",&delay_time);
    gets(tmptxt);
    sscanf(tmptxt,"ANS_FILENAME=%s",tmptxt);
    ANS_FILENAME=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"NOTE_FILENAME=%s",tmptxt);
    NOTE_FILENAME=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"IF_HAVE_MATLAB=%s",tmptxt);
    IF_HAVE_MATLAB=tmptxt[0]=='1';
    gets(tmptxt);
    sscanf(tmptxt,"AL_TOKEN=%s",tmptxt);
    AL_TOKEN=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"APP_ID=%s",tmptxt);
    APP_ID=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"APP_SECRET=%s",tmptxt);
    APP_SECRET=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"BAIDU_APP_ID=%s",tmptxt);
    BAIDU_APP_ID=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"BAIDU_APP_SECRET=%s",tmptxt);
    BAIDU_APP_SECRET=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"OPEN_AI_SECRET=%s",tmptxt);
    OPEN_AI_SECRET=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"OPEN_AI_MODEL=%s",tmptxt);
    OPEN_AI_MODEL=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"AI_PATH=%s",tmptxt);
    AI_PATH=tmptxt;
    gets(tmptxt);
    sscanf(tmptxt,"SYMBOL_NAME=%s",tmptxt);
    SYMBOL_NAME=tmptxt;

  }
  {
    char opt[1<<13]={0};
    info_print("Self_ID=");
    info_puts(Self_ID.c_str());
    info_print("REPORT_ID=");
    info_puts(REPORT_ID.c_str());
    sprintf(opt,"\rINPORT=%d",INPORT);
    info_puts(opt);
    sprintf(opt,"\rOUTPORT=%d",OUTPORT);
    info_puts(opt);
    sprintf(opt,"\rsleep_time=%d",sleep_time);
    info_puts(opt);
    sprintf(opt,"\rdelay_time=%d",delay_time);
    info_puts(opt);
    sprintf(opt,"\rANS_FILENAME=%s",ANS_FILENAME.c_str());
    info_puts(opt);
    sprintf(opt,"\rNOTE_FILENAME=%s",NOTE_FILENAME.c_str());
    info_puts(opt);
    sprintf(opt,"\rIF_HAVE_MATLAB=%d",IF_HAVE_MATLAB);
    info_puts(opt);
    sprintf(opt,"\rAL_TOKEN=%s",AL_TOKEN.c_str());
    info_puts(opt);
    sprintf(opt,"\rAPP_ID=%s",APP_ID.c_str());
    info_puts(opt);
    sprintf(opt,"\rAPP_SECRET=%s",APP_SECRET.c_str());
    info_puts(opt);
    sprintf(opt,"\rBAIDU_APP_ID=%s",BAIDU_APP_ID.c_str());
    info_puts(opt);
    sprintf(opt,"\rBAIDU_APP_SECRET=%s",BAIDU_APP_SECRET.c_str());
    info_puts(opt);
    sprintf(opt,"\rOPEN_AI_SECRET=%s",OPEN_AI_SECRET.c_str());
    info_puts(opt);
    sprintf(opt,"\rOPEN_AI_MODEL=%s",OPEN_AI_MODEL.c_str());
    info_puts(opt);
    sprintf(opt,"\rAI_PATH=%s",AI_PATH.c_str());
    info_puts(opt);
    sprintf(opt,"\rSYMBOL_NAME=%s",SYMBOL_NAME.c_str());
    info_puts(opt);
  }
}
void read_ans(){
  info_lable("[File_Read]");
  info_print("开始加载应答库\n");
  FILE *fp=freopen(ANS_FILENAME.c_str(),"r",stdin);
  Words temp;
  for(;;){
    getss(temp.txt);

    if(temp.crt("Key_st")||get_st(temp.txt,"spid=")==0||get_st(temp.txt,"sgid=")==0||temp.crt("Priv_key")){
      Group grp_tmp;
      if(get_st(temp.txt,"spid=")!=-1){
        grp_tmp.sp=1;
        char tmmp[15];
        memset(tmmp,0,sizeof tmmp);
        get_copy(5,strlen(temp.txt),temp.txt,tmmp);
        grp_tmp.spid=tmmp;
      }if(get_st(temp.txt,"sgid=")!=-1){
        grp_tmp.sg=1;
        char tmmp[15];
        memset(tmmp,0,sizeof tmmp);
        get_copy(5,strlen(temp.txt),temp.txt,tmmp);
        grp_tmp.spid=tmmp;
      }if(temp.crt("Priv_key")){
        grp_tmp.priv=1;
      }
      Keys tmp,tmp2;
      for(;;){
        getss(temp.txt);

        if(temp.crt("[emoji_草]")){
          memset(temp.txt,0,sizeof temp.txt);
          temp.txt[0]=-16;
          temp.txt[1]=-97;
          temp.txt[2]=-116;
          temp.txt[3]=-65;
          temp.txt[4]=0;
        }

        if(temp.crt("Key_1")){
          tmp.allcrt=1;
          grp_tmp.insert_key(tmp);
          tmp=tmp2;
        }else if(temp.crt("Key_0")){
          tmp.allcrt=0;
          grp_tmp.insert_key(tmp);
          tmp=tmp2;
        }else if(temp.crt("Key_ed"))break;
        else tmp.insert_word(temp);
      }for(;;){
        getss(temp.txt);
        if(temp.crt("Ans_or")){
          grp_tmp.insert_ans(tmp);
          all_ans_num++;
          tmp=tmp2;
        }else if(temp.crt("Pri")){
          grp_tmp.insert_ans(tmp);
          all_ans_num++;
          getss(temp.txt);
          sscanf(temp.txt,"%d",&grp_tmp.pri);
          grp.push_back(grp_tmp);
          grp_num++;
          break;
        }else tmp.insert_word(temp);
      }
    }
    else if(temp.crt("//end")){sort(grp.begin(),grp.begin()+grp_num);return;}
  }
  fflush(fp);
  fclose(stdin);
  //freopen("CON","r",stdin);
}
void read_white_list(){
  info_lable("[File_Read]");
  info_print("加载群聊白名单white_list.lst\n");
  freopen("white_list.lst","r",stdin);
  //info_print("白名单列表：\n");
  char txt[15];
  Grp_white_list[white_list_num++]="WHITE_LIST";
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      Grp_white_list[white_list_num++]=txt;
      //color_puts(txt);
    }
  }
  info_lable("[File_Read]");
  info_print("加载私聊广播名单broad_pri.lst\n");
  freopen("broad_pri.lst","r",stdin);
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      broad_pri[broad_pri_num++]=txt;
      //color_puts(txt);
    }
  }
  info_lable("[File_Read]");
  info_print("加载私聊配置名单pri_white_list.lst\n");
  freopen("pri_white_list.lst","r",stdin);
  //char txt[15];
  memset(txt,0,sizeof txt);
  gets(txt);
  Pri_blackorwhite=txt[0]=='b'?0:1;
  info_lable("[File_Read]");
  info_print(Pri_blackorwhite?"白名单\n":"黑名单\n");
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      Pri_white_list[Pri_white_list_num++]=txt;
      //color_puts(txt);
    }
  }
  info_lable("[File_Read]");
  info_print("加载全局用户黑名单black_list.lst\n");
  freopen("black_list.lst","r",stdin);
  //char txt[15];
  memset(txt,0,sizeof txt);
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      black_list[black_list_num++]=txt;
      //color_puts(txt);
    }
  }
}
void read_priv(){
  info_lable("[File_Read]");
  info_print("加载权限名单priv_list.lst\n");
  freopen("priv_list.lst","r",stdin);
  //color_print("权限列表：\n");
  char txt[15];
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      priv_list[priv_num++]=txt;
      //color_puts(txt);
    }
  }
}
void read(){
  read_config();
  read_ans();
  read_priv();
}
#endif
