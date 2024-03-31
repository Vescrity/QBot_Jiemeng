#ifndef JIEMENG_MESSAGE
#define JIEMENG_MESSAGE
#include <string>
#include <nlohmann/json.hpp>
#include "Jiemeng_CQJson.hpp"
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
  bool is_group() const { return group_flag; }
  bool is_private() const { return !group_flag; }
  bool operator==(const Message_Place &a) const { return (group_id == a.group_id) && (user_id == a.user_id); }
  bool operator!=(const Message_Place &a) const { return !(*this == a); }
};
/// @brief 消息类
class Message
{
  void message_init(const json &);
  void notice_init(const json &);

public:
  Message_Place place;
  CQMessage text;
  /// @brief 显示消息内容
  string & str(){return text.get_string();}
  void show() const;
  bool is_group() const { return place.is_group(); }
  bool is_private() const { return place.is_private(); }
  void init(const json &);
  CQMessage &message() { return text; }
};

#endif