/*******主程序*******/
#include "Jiemeng_Basic.h"
#include <stdio.h>
#include <string>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include "Socket_Input.h"
#include "Jie_Meng_Base.h"
#include "Msg_type.h"
#include "Special_Judge.h"

// #include"Key_Ans.h"
#include "File_Read.h"
#include "Daily.h"
#pragma comment(lib, "ws2_32.lib")
#define THR_NUM 128
using namespace std;
int thr_id;

struct thr_data
{
  Msg_type tp;
  // string txt;
  thr_data() {}
} dt[THR_NUM];
// 线程创建时直接传入结构体发现数据不能完整传入故定义全局
static int thrr[THR_NUM];
void Thread(const Msg_type &dta)
{
  // Msg_type dta=dt[thrid].tp;
  srand(time(NULL) + clock() + thr_id);
  Msg_type type = dta;
  // if(spj(type,type.msgs.c_str()))return;
  string str = get_output(type, type.msgs.c_str());
  if (str != "[FAILED]")
    return output(type, str.c_str());
  if (!type.ifgrp)
    return resend(type, "[default]");
  return;
}
static thread th1[128];
void Conv()
{
  func_show("Conv");
  char txxt[1 << 15] = {0};
  memset(txxt, 0, sizeof txxt);
  Msg_type type;
  type = init();
  if (type.if_ignore)
    goto ed;
  // if(type.grp_id[0]=='-')goto ed;
  th1[thr_id] = thread(Thread, type);
  th1[thr_id++].detach();
  if (thr_id >> 7)
    thr_id = 0;
ed:
  uni_sl(sleep_time);
}
void Heart_Beat()
{
#ifndef LINUX_V
  for (;;)
  {
    WinExec(SYMBOL_NAME.c_str(), SW_HIDE);
    uni_sl(10000);
  }
#else
  for (;;)
  {
    int tn = Main_thr_num.get_new_num();
    Main_thread_list[tn] = thread(system, string("./" + SYMBOL_NAME).c_str());
    uni_sl(10000);
  }
#endif
}

int main()
{
  int stt = clock();
#ifndef LINUX_V
  system("chcp 65001");
#endif
  if (!read())
  {
    error_lable("[File_Read]");
    error_puts("配置文件存在问题，无法启动，请调整后重试。");
    uni_sl(10000);
    return 0;
  }
  char tmp[1 << 12] = {0};

  execmd("pwd", tmp);
  // getcwd(tmp,sizeof tmp);
  // puts(tmp);
  strchg("\n", "", tmp);
  strchg("\r", "", tmp);
  RUN_PATH = tmp;

  read_white_list();
  freopen("CON", "r", stdin);

  int trytime = 0;
  while (wsainit() == -1)
  {
    trytime++;
    error_lable("[wsainit]");
    error_puts("bind失败");

    if (trytime > 10)
    {
      error_lable("[start_up]");
      error_puts("反复启动失败，程序结束。");
      uni_sl(10000);
      return 0;
    }
  }
  thread HB(Heart_Beat);
  HB.detach();
  thread DL(Daily);
  DL.detach();
  start_time++;

  int edt = clock();
#ifndef LINUX_V
  sprintf(tmp, "title %s", TITLE.c_str());
  string ttp = UTF8ToGBK(tmp);
  system(ttp.c_str());
#endif

  sprintf(tmp, "启动成功，启动耗时%dms\n", int((edt - stt) * 1000.0 / CLOCKS_PER_SEC));
  color_lable("[start_up]");
  color_puts(tmp);
  // print_test();

  for (; keep_run;)
  {
    Conv();
  }
}
