#ifndef JIEMENG_STATUS
#define JIEMENG_STATUS
#include "Jiemeng_Ans.h"
#include <iostream>
#include <sstream>
using namespace std;
string jm_status()
{
  int ans_size = answers.ans_group.size();
  std::ostringstream oss;

  std::string text = "Hello, world!";

  oss << "已加载词库: " << ans_size << "组" << endl;
#ifdef JIEMENG_DECK
  oss << "可用牌堆词条数: " << decks.size() << endl;
#endif
  return oss.str();
}

#endif