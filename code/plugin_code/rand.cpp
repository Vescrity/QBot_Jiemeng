/***************************

  * plugin模版v0.1

  **  注意事项：所有通过stdout流输出的内容都会作为输出消息

***************************/
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
const std::string PLUGIN_NAME = "rand";                    // 插件名称
const std::string PLUGIN_PATH = "./plugin/" + PLUGIN_NAME; // 插件路径，运行中产生的文件请保存到这个目录下
std::string plugin_function(std::string);
int main(int a, char **ss)
{
  FILE *fp;
  fp = fopen("Current_msg.txt", "r");
  int c;
  char ms[1 << 13] = {0};
  int i = 0;
  while ((c = fgetc(fp)) != EOF)
  {
    ms[i++] = c;
  }
  printf("%s", plugin_function(std::string(ms)).c_str());
  return 0;
}
#include "../opstring.h"
#include "../r_adb.h"
#include <time.h>
#include <random>
#include <chrono>
using namespace std;
std::string plugin_function(std::string msgs)
{
  char opt[1 << 8] = {0};
  msgs = msg_decode(msgs);
  int sl = get_st(msgs.c_str(), "[");
  int sr = msgs.length();
  char ss[1 << 10] = {0};
  get_copy(sl, sr, msgs.c_str(), ss);
  if (get_st(ss + 2, "[") != -1)
    return "INVALID EXPRESSION1";
  if (get_st(msgs.c_str(), ",") != -1)
  {

    long long l, r;
    sscanf(ss, "[%lld,%lld]", &l, &r);
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    uniform_int_distribution<long long> u(l, r);
    default_random_engine e(seed);
    sprintf(opt, "rand(%lld,%lld)=%lld", l, r, u(e));
    return opt;
  }

  get_copy(1, strlen(ss) - 1, ss, opt);
  string ans = opt;
  string Rc = chg_Rcode(ss);
  ans = ans + "=" + Rc;
  if (Rc[0] == '[')
    return "INVALID EXPRESSION2";
  else
    return ans;
}
