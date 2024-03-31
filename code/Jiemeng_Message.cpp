#include "Jiemeng_Message.hpp"
#include "Jiemeng_LogIO.hpp"
#include <iostream>
#include "Jiemeng_Algorithm.hpp"
using namespace std;
void Message::init(const json &js)
{
  if (js["post_type"] != "notice")
    message_init(js);
  else
    notice_init(js);
}
void Message::message_init(const json &js)
{
  place.group_flag = js["message_type"] == "group";
  text.change(string( js["raw_message"]));
  const json &sender = js["sender"];
  place.user_id = to_string(js["user_id"]);
  if (sender["card"].is_null())
    place.user_nm = sender["nickname"];
  else
    place.user_nm = sender["card"];
  if (is_group())
  {
    place.group_id = to_string(js["group_id"]);
    // TODO: get_group_name
  }
}
void Message::notice_init(const json &js)
{
  const string notice_type = js["notice_type"];
  string able_type[] = {"group_upload", "group_increase", "group_ban", "group_recall", "notify"};
  if (array_search(notice_type, able_type))
  {
    place.group_flag = 1;
    place.group_id = to_string(js["group_id"]);
    place.user_id = to_string(js["user_id"]);
    text = "[JM:";
    text.str() += notice_type;
    if (notice_type == "notify")
    {
      text = text.str() + ",subtype=" + to_string(js["sub_type"]);
    }
    text.str() += "]";
  }
  else
  {
    /// TODO
  }
}

void print_cutline()
{
  string hf = "----------------";
  color_print(hf.c_str());
  color_puts(hf.c_str());
}

void Message::show() const
{
  print_cutline();
  if (is_group())
  {
    info_lable("[group_nm]");
    info_puts(place.group_nm);
    info_lable("[group_id]");
    info_puts(place.group_id);
  }
  info_lable("[user_nm]");
  info_puts(place.user_nm);
  info_lable("[user_id]");
  info_puts(place.user_id);
  msg_lable("[message]");
  msg_puts(text);
}
