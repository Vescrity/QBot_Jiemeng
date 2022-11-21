/*******基本函数，全局变量定义******/
#ifndef JIE_MENG_BASE
  #define JIE_MENG_BASE
  #ifndef IP_ADD
    #define IP_ADD "http://127.0.0.1:"+string(pt)+"/"
  #endif
  #include<string.h>
#include<string>
#include<vector>
#include"opstring.h"
#include<map>
using namespace std;
int grp_num,white_list_num,Pri_white_list_num,
  INPORT,OUTPORT,black_list_num,sleep_time,start_time,
  msg_send_time,all_ans_num,priv_num,last_AI_time;
map<string,int>time_list;
bool Pri_blackorwhite;//0b1w
bool main_switch=1,keep_run=1,if_AI_run=0;
bool report=0;
string Current_Msg,Self_ID,ANS_FILENAME,
  APP_ID,APP_SECRET,AI_PATH,SYMBOL_NAME,
  BAIDU_APP_ID,BAIDU_APP_SECRET;
string Grp_white_list[128],Pri_white_list[128],black_list[128],priv_list[128];
bool execmd(const char* cmd,char* result) {
      char buffer[128];                                     //定义缓冲区
      FILE* pipe = _popen(cmd,"r");                  //打开管道，并执行命令
      if (!pipe)
          return 0;                                 //返回0表示运行失败
      int cnt=0;
      while(!feof(pipe)&&cnt<100) {//10
      if(fgets(buffer,128,pipe)){                   //将管道输出到result中
                  strcat(result,buffer);
                  cnt++;
            }
      }
      _pclose(pipe);                                          //关闭管道
      return 1;                                                 //返回1表示运行成功
}
void stats_refresh(){//刷新统计信息
  try{
    FILE* fopt;
    fopt=fopen("stats.data","w");
    fprintf(fopt,"start_time=%d\n",start_time);
    fprintf(fopt,"msg_send_time=%d\n",msg_send_time);
    fclose(fopt);
    fflush(stdout);
  }
  catch(...){
    puts("ERROR_in_stats_refresh()\n");
  }
}
bool check_priv(const char*id){//鉴权函数
  for(int i=0;i<priv_num;i++){
    if(pfct(id,priv_list[i].c_str()))return 1;
  }return 0;
}
#include"Key_Ans.h"
vector<Group> grp;
void Msg_type::get_grp_name(char*name){//获取群聊名称
  char pt[6];
  sprintf(pt,"%d",OUTPORT);
  char bf[1024];
  memset(bf,0,sizeof bf);
  string ord="curl ";ord=ord+IP_ADD+"get_group_info?group_id="+grp_id;
  execmd(ord.c_str(),bf);
  int l=get_st(bf,"\"group_name");
  l+=14;
  int r=get_st(bf+l,"\",\"");
  r+=l;
  get_copy(l,r,bf,name);
  name[r-l]=0;
}
#include"Special_Judge.h"
bool spj(Msg_type,const char*,const char*);//特殊功能
void resend(Msg_type type,const char*msg){
  if(spj(type,msg,msg))return;
  for(int i=0;i<grp_num;i++){
    if(grp[i].crt(type,msg)){
      grp[i].print(type);
      break;
    }
  }
}
#endif
