#ifndef JIE_MENG_BASE
  #define JIE_MENG_BASE
  /**bot相关较基本函数与全局变量**/
  #ifndef IP_ADD
    #define IP_ADD "http://127.0.0.1:"+string(pt)+"/"
  #endif
  #include<string.h>
#include<string>
#include<vector>
#include"opstring.h"
#include<map>
using namespace std;
int grp_num/*应答组总量*/,white_list_num/*群聊白名单数量*/,Pri_white_list_num/*私聊名单人数*/,INPORT/*上报端口*/,OUTPORT/*输出端口*/,black_list_num/*全局黑名单人数*/,sleep_time/*冷却时长*/,start_time/*启动次数统计*/,msg_send_time/*消息发送次数统计*/,all_ans_num/*总回复数统计*/,priv_num/*有权限人数*/;
map<string,int>time_list;/*计时器列表*/
bool Pri_blackorwhite;//0b1w
bool main_switch=1,keep_run=1;/*总开关与是否继续运行开关*/
string Current_Msg,Self_ID,ANS_FILENAME;                                        //当前实际收到消息，自身qq号，应答库文件名
string Grp_white_list[128],Pri_white_list[128],black_list[128],priv_list[20];   //相关数据存储表
bool execmd(const char* cmd,char* result) {                                     //一个获取cmd指令反馈内容的函数
      char buffer[128];                                     //定义缓冲区
      FILE* pipe = _popen(cmd,"r");                  //打开管道，并执行命令
      if (!pipe)
          return 0;                                 //返回0表示运行失败
      int cnt=0;
      while(!feof(pipe)&&cnt<30) {//10
      if(fgets(buffer,128,pipe)){                   //将管道输出到result中
                  strcat(result,buffer);
                  cnt++;
            }
      }
      _pclose(pipe);                                          //关闭管道
      return 1;                                                 //返回1表示运行成功
}
void stats_refresh(){                                                           //统计数据文件更新
  FILE* fopt;
  fopt=fopen("stats.data","w");
  //freopen("stats.data","w",fopt);
  fprintf(fopt,"start_time=%d\n",start_time);
  fprintf(fopt,"msg_send_time=%d\n",msg_send_time);
  fclose(fopt);
  fflush(stdout);
  //freopen("CON","w",stdout);
  //puts("back");
}
bool check_priv(const char*id){                                                 //权限检查
  for(int i=0;i<priv_num;i++){
    if(pfct(id,priv_list[i].c_str()))return 1;
  }return 0;
}
#include"Key_Ans.h"
vector<Group> grp;
void Msg_type::get_grp_name(char*name){
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
bool spj(Msg_type,const char*,const char*);                                     //是否触发千奇百怪功能的判断函数
void resend(Msg_type type,const char*msg){                                      //等效收到消息并回答
  if(spj(type,msg,msg))return;
  for(int i=0;i<grp_num;i++){
    if(grp[i].crt(type,msg)){
      grp[i].print(type);
      break;
    }
  }
}
#endif
