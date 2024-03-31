#ifndef JIEMENG_MESSAGE
#define JIEMENG_MESSAGE
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using string = std::string;
/// @brief 消息位置
class Message_Place
{
public:
  bool group_flag;
  string group_id;
  string group_nm;
  string user_id;
  string user_nm;
  bool is_group() { return group_flag; }
  bool is_private() { return !group_flag; }
  bool operator==(const Message_Place &a) { return (group_id == a.group_id) && (user_id == a.user_id); }
  bool operator!=(const Message_Place &a) { return !(*this == a); }
};
/// @brief 消息类
class Message
{
  Message_Place place;
  string text;

public:
  /// @brief 显示消息内容
  void show();
  bool is_group() { return place.is_group(); }
  bool is_private() { return place.is_private(); }
  void init(const json &);
};

#endif