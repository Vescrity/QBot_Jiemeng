#ifndef FILE_READ
  #define FILE_READ
#include<algorithm>
#include<fstream>
#include"Jie_Meng_Base.h"
#include"Ans.h"
void line_get(char *s){
  gets(s);

  if(s[strlen(s)-1]==13)s[strlen(s)-1]=0;
  if(s[0]=='#'||s[0]=='\0')line_get(s);
}
void read_stats(){
  info_lable("[File_Read]");
  info_puts("读取统计信息");
  freopen("stats.data","r",stdin);
  char tmptxt[50];
  line_get(tmptxt);
  STAT_START_TIME=tmptxt;
  line_get(tmptxt);
  sscanf(tmptxt,"start_time=%d",&start_time);
  line_get(tmptxt);
  sscanf(tmptxt,"msg_send_time=%d",&msg_send_time);
}
bool read_config(){
  read_stats();
  info_lable("[File_Read]");
  info_puts("加载配置文件");
  freopen("config.cfg","r",stdin);
  {
    char tmptxt[100];
    memset(tmptxt,0,sizeof tmptxt);
    line_get(tmptxt);
    if(get_st(tmptxt,"Self_ID")==-1){
      Self_ID="0";
    }else{
      sscanf(tmptxt,"Self_ID=%s",tmptxt);
      Self_ID=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"REPORT_ID")==-1){
      REPORT_ID="0";
    }else{
      sscanf(tmptxt,"REPORT_ID=%s",tmptxt);
      REPORT_ID=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"REPORT_GROUP")==-1){
      REPORT_GROUP="0";
    }else{
      sscanf(tmptxt,"REPORT_GROUP=%s",tmptxt);
      REPORT_GROUP=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"INPORT")==-1){
      return 0;
    }else{
      sscanf(tmptxt,"INPORT=%d",&INPORT);
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"OUTPORT")==-1){
      return 0;
    }else{
      sscanf(tmptxt,"OUTPORT=%d",&OUTPORT);
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"ENCRYPT_PASSWORD")==-1){
      ENCRYPT_PASSWORD=1234;
    }else{
      sscanf(tmptxt,"ENCRYPT_PASSWORD=%d",&ENCRYPT_PASSWORD);
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"sleep_time")==-1){
      sleep_time=0;
    }else{
      sscanf(tmptxt,"sleep_time=%d",&sleep_time);
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"delay_time")==-1){
      delay_time=0;
    }else{
      sscanf(tmptxt,"delay_time=%d",&delay_time);
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"TITLE")==-1){
      TITLE="桔梦"+Self_ID;
    }else{
      sscanf(tmptxt,"TITLE=%s",tmptxt);
      TITLE=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"ANS_FILENAME")==-1){
      ANS_FILENAME="Ans.json";
    }else{
      sscanf(tmptxt,"ANS_FILENAME=%s",tmptxt);
      ANS_FILENAME=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"NOTE_FILENAME")==-1){
      NOTE_FILENAME="note.txt";
    }else{
      sscanf(tmptxt,"NOTE_FILENAME=%s",tmptxt);
      NOTE_FILENAME=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"IF_HAVE_MATLAB")==-1){
      IF_HAVE_MATLAB=0;
    }else{
      sscanf(tmptxt,"IF_HAVE_MATLAB=%s",tmptxt);
      IF_HAVE_MATLAB=(tmptxt[0]=='1');
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"AL_TOKEN=")==-1){
      AL_TOKEN="0";
    }else{
      sscanf(tmptxt,"AL_TOKEN=%s",tmptxt);
      AL_TOKEN=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"APP_ID=")==-1){
      APP_ID="0";
    }else{
      sscanf(tmptxt,"APP_ID=%s",tmptxt);
      APP_ID=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"APP_SECRET=")==-1){
      APP_SECRET="0";
    }else{
      sscanf(tmptxt,"APP_SECRET=%s",tmptxt);
      APP_SECRET=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"BAIDU_APP_ID=")==-1){
      BAIDU_APP_ID="0";
    }else{
      sscanf(tmptxt,"BAIDU_APP_ID=%s",tmptxt);
      BAIDU_APP_ID=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"BAIDU_APP_SECRET=")==-1){
      BAIDU_APP_SECRET="0";
    }else{
      sscanf(tmptxt,"BAIDU_APP_SECRET=%s",tmptxt);
      BAIDU_APP_SECRET=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"OPEN_AI_SECRET=")==-1){
      OPEN_AI_SECRET="0";
    }else{
      sscanf(tmptxt,"OPEN_AI_SECRET=%s",tmptxt);
      OPEN_AI_SECRET=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"OPEN_AI_MODEL=")==-1){
      OPEN_AI_MODEL="0";
    }else{
      sscanf(tmptxt,"OPEN_AI_MODEL=%s",tmptxt);
      OPEN_AI_MODEL=tmptxt;
      line_get(tmptxt);
    }
    if(get_st(tmptxt,"AI_PATH=")==-1){
      AI_PATH="0";
    }else{
      sscanf(tmptxt,"AI_PATH=%s",tmptxt);
      AI_PATH=tmptxt;
      line_get(tmptxt);
    }

    if(get_st(tmptxt,"SYMBOL_NAME=")==-1){
      return 0;
    }else{
      sscanf(tmptxt,"SYMBOL_NAME=%s",tmptxt);
      SYMBOL_NAME=tmptxt;
      //line_get(tmptxt);
    }



  }
  {
    char opt[1<<13]={0};
    info_print("Self_ID=");
    info_puts(Self_ID.c_str());
    info_print("REPORT_ID=");
    info_puts(REPORT_ID.c_str());
    info_print("REPORT_GROUP=");
    info_puts(REPORT_GROUP.c_str());
    sprintf(opt,"\rINPORT=%d",INPORT);
    info_puts(opt);
    sprintf(opt,"\rOUTPORT=%d",OUTPORT);
    info_puts(opt);
    sprintf(opt,"\rENCRYPT_PASSWORD=%d",ENCRYPT_PASSWORD);
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
  }return 1;
}
void read_ans(){
  info_lable("[File_Read]");
  info_print("开始加载应答库\n");
  ifstream fread(ANS_FILENAME);
  json js=json::parse(fread);
  json ss=js["Ans_grp"];
  Ans_init(ss);
  info_lable("[File_Read]");
  info_print("应答库加载成功！\n");
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
    line_get(txt);
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
    line_get(txt);
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
  line_get(txt);
  Pri_blackorwhite=txt[0]=='b'?0:1;
  info_lable("[File_Read]");
  info_print(Pri_blackorwhite?"配置设置为白名单\n":"配置设置为黑名单\n");
  for(;;){
    memset(txt,0,sizeof txt);
    line_get(txt);
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
    line_get(txt);
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
    line_get(txt);
    if(strlen(txt)){
      if(txt[0]=='/')break;
      priv_list[priv_num++]=txt;
      //color_puts(txt);
    }
  }
}
#define IF_FP_OK \
if(fp==NULL)return 0;
bool save_config(){
  info_lable("[File_Save]");
  info_puts("开始保存名单配置");
  FILE*fp;
  fp=fopen("white_list.lst","w");IF_FP_OK
  for(int i=1;i<white_list_num;i++){
    fprintf(fp,"%s\n",Grp_white_list[i].c_str());
  }
  fprintf(fp,"//end");
  fclose(fp);
  fp=fopen("pri_white_list.lst","w");IF_FP_OK
  fprintf(fp,"%s\n",Pri_blackorwhite?"white":"black");
  for(int i=0;i<Pri_white_list_num;i++){
    fprintf(fp,"%s\n",Pri_white_list[i].c_str());
  }
  fprintf(fp,"//end");
  fclose(fp);
  fp=fopen("black_list.lst","w");IF_FP_OK
  for(int i=0;i<black_list_num;i++){
    fprintf(fp,"%s\n",black_list[i].c_str());
  }
  fprintf(fp,"//end");
  fclose(fp);
  return 1;
}
bool read(){
  if(!read_config())return 0;
  read_ans();
  read_priv();
  return 1;
}
#endif
