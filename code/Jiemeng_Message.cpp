#include "Jiemeng_Message.hpp"
#include "Jiemeng_LogIO.hpp"
#include <iostream>
using namespace std;
void Message::init(const json &js)
{
  place.group_flag = js["message_type"] == "group";
  text = js["raw_message"];
  const json &sender = js["sender"];
  place.user_id = to_string(js["user_id"]);
  if (sender["card"].is_null())
    place.user_nm = sender["nickname"];
  else
    place.user_nm = sender["card"];
  if (is_group())
  {
    place.group_id = to_string(js["group_id"]);
    // get_group_name
  }
}

void print_cutline()
{
  string hf = "----------------";
  color_print(hf.c_str());
  color_puts(hf.c_str());
}

void Message::show()
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
