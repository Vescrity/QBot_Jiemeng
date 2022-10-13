#ifndef FILE_READ
  #define FILE_READ
  /**配置文件加载库**/
#include<algorithm>
#include"Jie_Meng_Base.h"
#include"Key_Ans.h"
void getss(char *s){
  gets(s);
  if(s[0]=='#'||s[0]=='\0')getss(s);
}
void read_stats(){
  printf("\n读取统计信息");
  freopen("stats.data","r",stdin);
  char tmptxt[50];
  gets(tmptxt);
  sscanf(tmptxt,"start_time=%d",&start_time);
  gets(tmptxt);
  sscanf(tmptxt,"msg_send_time=%d",&msg_send_time);
}
void read_config(){
  read_stats();
  printf("\n加载配置文件\n");
  freopen("config.cfg","r",stdin);
  char tmptxt[100];
  memset(tmptxt,0,sizeof tmptxt);
  gets(tmptxt);
  sscanf(tmptxt,"Self_ID=%s",tmptxt);
  Self_ID=tmptxt;
  printf("Self_ID=");
  puts(Self_ID.c_str());
  gets(tmptxt);
  sscanf(tmptxt,"INPORT=%d",&INPORT);
  printf("INPORT=%d\n",INPORT);
  gets(tmptxt);
  sscanf(tmptxt,"OUTPORT=%d",&OUTPORT);
  printf("OUTPORT=%d\n",OUTPORT);
  gets(tmptxt);
  sscanf(tmptxt,"sleep_time=%d",&sleep_time);
  printf("sleep_time=%d\n",sleep_time);
  gets(tmptxt);
  sscanf(tmptxt,"ANS_FILENAME=%s",tmptxt);
  ANS_FILENAME=tmptxt;
  printf("应答库文件名：ANS_FILENAME=");
  puts(ANS_FILENAME.c_str());
}
void read_ans(){
  printf("\n开始加载应答库\n");
  freopen(ANS_FILENAME.c_str(),"r",stdin);
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
}
void read_white_list(){
  printf("\n加载群聊白名单white_list.lst\n");
  freopen("white_list.lst","r",stdin);
  printf("白名单列表：\n");
  char txt[15];
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      Grp_white_list[white_list_num++]=txt;
      puts(txt);
    }
  }printf("white_list.lst加载成功\n");
  printf("加载私聊配置名单pri_white_list.lst\n");
  freopen("pri_white_list.lst","r",stdin);
  //char txt[15];
  memset(txt,0,sizeof txt);
  gets(txt);
  Pri_blackorwhite=txt[0]=='b'?0:1;
  printf(Pri_blackorwhite?"白名单列表：\n":"黑名单列表：\n");
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      Pri_white_list[Pri_white_list_num++]=txt;
      puts(txt);
    }
  }printf("pri_white_list.lst加载成功\n");
  printf("加载全局用户黑名单black_list.lst\n");
  freopen("black_list.lst","r",stdin);
  //char txt[15];
  memset(txt,0,sizeof txt);

  printf("当前全局黑名单列表：\n");
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      black_list[black_list_num++]=txt;
      puts(txt);
    }
  }printf("black.lst加载成功\n");
}
void read_priv(){
  printf("\n加载权限名单priv_list.lst\n");
  freopen("priv_list.lst","r",stdin);
  printf("权限列表：\n");
  char txt[15];
  for(;;){
    memset(txt,0,sizeof txt);
    gets(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      priv_list[priv_num++]=txt;
      puts(txt);
    }
  }printf("priv_list.lst加载成功\n");
}
void read(){
  read_config();
  read_ans();
  read_priv();
}
#endif
