#ifndef JIEMENG_DECK
#define JIEMENG_DECK

#include "opstring.h"
#include <time.h>
#include "r_adb.h"
#include "Jiemeng_Exception.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;
class Deck
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
        } while (mp[ss][rjs]); // 检查是否允许抽取
        /// 标记是否允许再次抽取
        mp[ss][rjs] = if_flg;
        /// 递归式获取结果
        opt = opt + get_output(string(js[ss][rjs]));
      }
      catch (std::exception &e)
      {
        JM_EXCEPTION("[Deck]")
      }

      /// 向后移位
      str = str.c_str() + r + 1;
      l = get_st(str.c_str(), "{");
    }
    /// 合并余下部分
    return opt + str;
  }

public:
  /// @brief 初始化牌堆json
  /// @param folderPath 牌堆路径
  void init(const string &folderPath)
  {
    json mergedJSON;
    for (const auto &entry : fs::directory_iterator(folderPath))
    {
      if (entry.path().extension() == ".json")
      {
        ifstream file(entry.path());
        if (file)
        {
          json jsonData;
          file >> jsonData;
          file.close();
          for (auto it = jsonData.begin(); it != jsonData.end(); ++it)
            mergedJSON[it.key()] = it.value();
        }
      }
    }

    js = mergedJSON;
  }
  /// @brief 以默认路径(./deck)初始化牌堆json
  void init() { init("./deck"); }
  string draw(const string &str)
  {
    return get_output(string("{%") + str + "}");
    mp.clear();
  }
} decks;

#endif