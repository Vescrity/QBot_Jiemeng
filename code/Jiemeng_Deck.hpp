#ifndef JIEMENG_DECK
#define JIEMENG_DECK
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
  /// @param key json 键值
  /// @return 索引，返回随机结果对应的索引
  int r_js(const string &key);
  string get_output(string str);

public:
  Deck();
  int size() { return js.size(); }
  /// @brief 初始化牌堆json
  /// @param folderPath 牌堆路径
  void init(const string &folderPath);
  /// @brief 抽取
  /// @param str 键值
  /// @param times 次数
  /// @return 抽取结果
  string draw(const string &str, int times = 1);
  string list();
};

#endif