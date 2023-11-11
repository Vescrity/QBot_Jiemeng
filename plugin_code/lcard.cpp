/***************************

  *   plugin模版v0.1

  **  注意事项：所有通过stdout流输出的内容都会作为输出消息

***************************/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
const std::string PLUGIN_NAME = "card";                    // 插件名称
const std::string PLUGIN_PATH = "./plugin/" + PLUGIN_NAME; // 插件路径，运行中产生的文件请保存到这个目录下
std::string plugin_function();
int main(int a, char **ss)
{
  // WinExec("chcp 65001",SW_HIDE);
  //FILE *fp;
  //fp = fopen("Current_msg.txt", "r");
  /*int c;
  char ms[1 << 13] = {0};
  int i = 0;
  while ((c = fgetc(fp)) != EOF)
  {
    ms[i++] = c;
  }*/
  printf("%s", plugin_function().c_str());
  return 0;
}
#include "../opstring.h"
#include <time.h>
#include <random>
#include <chrono>
#include <fstream>
#include <time.h>
#include "../json.hpp"
using namespace std;
using json = nlohmann::json;

auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
default_random_engine e(seed);
int Rands(int l, int r)
{
  uniform_int_distribution<int> u(l, r);
  return u(e);
}
struct Card
{
  json js;
  map<string, map<int, bool>> mp;

  int r_js(const char *ss)
  {
    int n = js[ss].size();
    string s;
    int n_lst[n + 1] = {0};
    int t;
    for (int i = 0; i < n; i++)
    {
      s = js[ss][i];
      t = 1;
      if (get_st(s.c_str(), "::") == 0)
      {
        sscanf(s.c_str(), "::%d::", &t);
      }
      n_lst[i + 1] = n_lst[i] + t;
    }
    int r = Rands(0, n_lst[n] - 1);
    for (int i = 1; i <= n; i++)
    {
      if (n_lst[i] > r)
        return i - 1;
    }
    return n - 1;
  }
  string get_output(string str)
  {

    if (get_st(str.c_str(), "::") == 0)
    {
      int q = get_st(str.c_str() + 2, "::");
      str = str.c_str() + q + 4;
    }
    string opt = "";
    int l, r;
    bool if_flg = 0;
    l = get_st(str.c_str(), "{");
    if (str[l + 1] != '%')
    {
      if_flg = 1;
    }
    char ss[1 << 12];
    while (l != -1)
    {
      get_copy(0, l, str.c_str(), ss);
      opt += ss;
      r = get_st(str.c_str() + l, "}") + l;
      l += 2;
      l -= if_flg;
      get_copy(l, r, str.c_str(), ss);

      int rjs;
      int clc = clock();
      do
      {
        rjs = r_js(ss);
        if ((clock() - clc) * 1000.0 / CLOCKS_PER_SEC > 100)
          return "TIME_OUT";
      } while (mp[ss][rjs]);
      mp[ss][rjs] = if_flg;

      opt = opt + get_output(string(js[ss][rjs]));
      str = str.c_str() + r + 1;
      l = get_st(str.c_str(), "{");
      if (str[l + 1] != '%')
      {
        if_flg = 1;
      }
      else
        if_flg = 0;
    }
    return opt + str;
  }
};
string txxts[1 << 14];
Card c;

std::string plugin_function()
{
  // char opt[1<<8]={0};
  string opt = "";
  /*msgs = msg_decode(msgs);
  int sl = get_st(msgs.c_str(), "#");
  int sr = get_st(msgs.c_str(), "~");
  if (sr == -1)
    return "ERROR";
  int ll = msgs.length();*/
  char name[1 << 6] = {0};
  char paras[1 << 9] = {0};/*
  get_copy(sl + 1, sr, msgs.c_str(), name);
  get_copy(sr + 1, ll, msgs.c_str(), paras);
  int t;
  //sscanf(paras, "%d", &t);
  t=1;
  //opt+=name;
  if (t <= 0 || t > 10)
    return "ERROR";
    */
  int t = 1;
  string fnm = "ask.json";
  string para = "ask";
  std::ifstream f(fnm);
  c.js = json::parse(f);
  /*sl = get_st(msgs.c_str() + sr + 1, "~");
  if (sl != -1)
  {
    sl += sr + 1;
    get_copy(sl + 1, ll, msgs.c_str(), paras);
    // puts(paras);
    para = paras;
  }
*/
  for (int i = 0; i < t; i++)
  {
    opt = opt + c.get_output(string("{%") + para + "}") + ((i != t - 1) ? "\n" : "");
    c.mp.clear();
  }
  return opt;
}
