/*******基本函数，全局变量定义******/
#ifndef JIE_MENG_BASE
  #define JIE_MENG_BASE
  #ifndef IP_ADD
    #define IP_ADD "http://127.0.0.1:"+string(pt)+"/"
  #endif
  #include<string.h>
  #include<string>
  #include<vector>
  #include<map>
  #include"Jiemeng_Basic.h"

  using namespace std;
  int white_list_num,Pri_white_list_num,
    INPORT,OUTPORT,black_list_num,sleep_time,delay_time,start_time,broad_pri_num,
    msg_send_time,all_ans_num,priv_num,last_AI_time;
  map<string,int>time_list;
  bool Pri_blackorwhite;//0b1w
  bool main_switch=1,keep_run=1,IF_HAVE_MATLAB;
  bool report=0;
  string Self_ID,ANS_FILENAME,REPORT_ID,REPORT_GROUP,STAT_START_TIME,
    APP_ID,APP_SECRET,AI_PATH,SYMBOL_NAME,NOTE_FILENAME,
    BAIDU_APP_ID,BAIDU_APP_SECRET,AL_TOKEN,RUN_PATH,OPEN_AI_SECRET,OPEN_AI_MODEL;
  string Grp_white_list[128],Pri_white_list[128],black_list[128],priv_list[128],broad_pri[128];
  struct Bot_Self_Judge{
    int it;
    int list[5];
    Bot_Self_Judge(){it=0;memset(list,0,sizeof list);}
    void push(string s_id){
      int id;
      sscanf(s_id.c_str(),"%d",&id);
      if(id!=0)list[it++]=id;
      if(it==5)it=0;
    }
    bool judge_not_bot(int id){
      //printf("judge_not_bot,id=%d\n",id);

      for(int i=0;i<5;i++){
        //printf("list[%d]=%d\n",i,list[i]);
        if(id==list[i]&&id!=0)return 0;
      }
      return 1;
    }
  }Bot_Judge;
  void stats_refresh(){//刷新统计信息
    try{
      FILE* fopt;
      fopt=fopen("stats.data","w");
      fprintf(fopt,"%s\n",STAT_START_TIME.c_str());
      fprintf(fopt,"start_time=%d\n",start_time);
      fprintf(fopt,"msg_send_time=%d\n",msg_send_time);
      fclose(fopt);
      fflush(stdout);
    }
    catch(...){
      error_puts("ERROR_in_stats_refresh()");
    }
  }
  bool check_priv(const char*id){//鉴权函数
    for(int i=0;i<priv_num;i++){
      if(pfct(id,priv_list[i].c_str()))return 1;
    }return 0;
  }
  #include"Key_Ans.h"

  void Msg_type::get_grp_name(){//获取群聊名称
    char pt[6];
    sprintf(pt,"%d",OUTPORT);
    char bf[1024];
    memset(bf,0,sizeof bf);
    string ord="curl -s ";ord=ord+IP_ADD+"get_group_info?group_id="+grp_id;
    execmd(ord.c_str(),bf);
    this->grp_name=json_read(bf,"group_name");
  }
  bool spj(Msg_type,const char*,const char*);
  void resend(Msg_type type,const char*msg);
  /*bool spj(Msg_type,const char*,const char*);//特殊功能
  void resend(Msg_type type,const char*msg){
    if(spj(type,msg,msg))return;
    for(int i=0;i<grp_num;i++){
      if(grp[i].crt(type,msg)){
        grp[i].print(type);
        break;
      }
    }
  }*/string chg_resend(Msg_type type,const char*msg){
    for(int i=0;i<grp_num;i++){
      if(grp[i].crt(type,msg)){
        return grp[i].get_opt();
      }
    }return "";
  }
#endif
