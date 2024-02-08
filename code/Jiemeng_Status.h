#ifndef JIEMENG_STATUS
#define JIEMENG_STATUS
#include "Jiemeng_Ans.h"
#include <iostream>
#include <sstream>
#include "Time_Struct.h"
using namespace std;

str_tm start_time;

string jm_status()
{
  int ans_size = answers.ans_group.size();
  std::ostringstream oss;
  oss << "启动时间：" << start_time.year << "-" << start_time.month << "-" << start_time.mday
      << " " << start_time.hour << ":" << start_time.min << ":" << start_time.sec << endl;
  oss << "已加载词库: " << ans_size << "组" << endl;
#ifdef JIEMENG_DECK
  oss << "可用牌堆词条数: " << decks.size();
#endif
  return oss.str();
}

#endif