#ifndef JIEMENG_MESSAGE
#define JIEMENG_MESSAGE
#include <string>
#include <nlohmann/json.hpp>
#include "Jiemeng_CQJson.hpp"
using json = nlohmann::json;
using string = std::string;
class Config;
class Message;
/// @brief 消息位置
class Message_Place
{
  bool group_flag;

public:
  string group_id;
  string group_nm;
  string user_id;
  string user_nm;
  int level;
  void get_level(Config *);
  void set_private() { group_flag = 0; }
  void set_group() { group_flag = 1; }
  bool is_group() const { return group_flag; }
  bool is_private() const { return !group_flag; }
  bool operator==(const Message_Place &a) const { return (group_id == a.group_id) && (user_id == a.user_id); }
  bool operator!=(const Message_Place &a) const { return !(*this == a); }
  friend class Message;
};
/// @brief 消息类
class Message
{
  void message_init(const json &);
  void notice_init(const json &);

public:
  Message_Place place;
  CQMessage text;

  string &str() { return text.get_string(); }
  /// @brief 显示消息内容
  void show() const;
  void change(const string &s) { text.change(s); }
  string true_str() { return text.true_str(); }
  bool is_group() const { return place.is_group(); }
  bool is_private() const { return place.is_private(); }
  void init(const json &);
  CQMessage &message() { return text; }
};

#endif