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
std::string plugin_function(std::string);
int main(int a, char **ss)
{
  // WinExec("chcp 65001",SW_HIDE);
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
#include <time.h>
#include <random>
#include <chrono>
#include <fstream>
#include <time.h>
#include <nlohmann/json.hpp>
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
  /// @brief 维护标记不得再次抽取
  map<string, map<int, bool>> mp;

  /// @brief 随机含权数组
  /// @param ss json 键值
  /// @return 索引，返回随机结果对应的索引
  int r_js(const char *ss)
  {
    int n = js[ss].size();
    if (n == 0)
      throw invalid_argument(string(ss) + " not found.");
    string s;
    int n_lst[n + 1];
    memset(n_lst, 0, sizeof n_lst);
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
#ifdef DEBUG
    cerr << endl
         << "=>get_output():" << endl
         << str << endl
         << endl;
#endif
    if (get_st(str.c_str(), "::") == 0)
    {
      int q = get_st(str.c_str() + 2, "::");
      str = str.c_str() + q + 4;
    }
    string opt = "";
    int l, r;

    /// 由 `%` 而需用来调节偏移
    /// 同时用来控制是否重复抽取，含%表示可重复抽取，否则不重复抽取
    bool if_flg = 0;
    /// 获取复制起点
    l = get_st(str.c_str(), "{");
    char ss[1 << 12];
    while (l != -1)
    {
      if_flg = (str[l + 1] != '%');
      get_copy(0, l, str.c_str(), ss);
      opt += ss;
      r = get_st(str.c_str() + l, "}") + l;
      l += 2 - if_flg;
      get_copy(l, r, str.c_str(), ss);
      int rjs;
      int clc = clock();
      try
      {
        do
        {
          /// 抽取索引
          rjs = r_js(ss);
          if ((clock() - clc) * 1000.0 / CLOCKS_PER_SEC > 100)
            return "TIME_OUT";
        } while (!mp[ss][rjs]); // 检查是否允许抽取
        /// 标记是否允许再次抽取
        mp[ss][rjs] = if_flg;
        /// 递归式获取结果
        opt = opt + get_output(string(js[ss][rjs]));
      }
      catch (std::exception &e)
      {
        std::string msg = "Exception caught: ";
        msg += e.what();
        cerr << msg << endl;
        opt = opt + "{" + ss + "}";
      }

      /// 向后移位
      str = str.c_str() + r + 1;
      l = get_st(str.c_str(), "{");
    }
    /// 合并余下部分
    return opt + str;
  }
};
string txxts[1 << 14];
Card c;

std::string plugin_function(std::string msgs)
{
  // char opt[1<<8]={0};
  string opt = "";
  msgs = msg_decode(msgs);
  int sl = get_st(msgs.c_str(), "#");
  int sr = get_st(msgs.c_str(), "~");
  if (sr == -1)
    return "ERROR";
  int ll = msgs.length();
  char name[1 << 6] = {0};
  char paras[1 << 9] = {0};
  get_copy(sl + 1, sr, msgs.c_str(), name);
  get_copy(sr + 1, ll, msgs.c_str(), paras);
  int t;
  sscanf(paras, "%d", &t);

  // opt+=name;
  if (t <= 0 || t > 10)
    return "ERROR";
  string fnm = PLUGIN_PATH + "/" + name + ".json";
  string para = name;
  std::ifstream f(fnm);
  c.js = json::parse(f);
  sl = get_st(msgs.c_str() + sr + 1, "~");
  if (sl != -1)
  {
    sl += sr + 1;
    get_copy(sl + 1, ll, msgs.c_str(), paras);
    // puts(paras);
    para = paras;
  }

  for (int i = 0; i < t; i++)
  {
    opt = opt + c.get_output(string("{%") + para + "}") + ((i != t - 1) ? "\n" : "");
    c.mp.clear();
  }
  return opt;
}
