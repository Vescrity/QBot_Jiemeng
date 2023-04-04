#ifndef MESSAGE_INPUT
#define MESSAGE_INPUT

#include "Message_Type.h"
#include "Bot_Send_Check.h"
#include "json.hpp"
#include <string>
#include "Jiemeng_Config.h"
using json = nlohmann::json;
using namespace std;


void Message_Type::init(const string &msg)
{
  // info_puts(msg.c_str());
  json Event = json::parse(msg);
  // btype.ifgroup = Event.count("group_id");
  json &post_type = Event["post_type"];
  int &admin_level = btype.admin_level;
  if (post_type == "message" || post_type == "message_sent")
  {
    if (post_type == "message_sent")
    {
      // 是否bot发出检查
      white = bot_check.check(Event["message_id"]);
    }
    btype.ifgroup = Event["message_type"] == "group";
    btype.message = Event["raw_message"];
    json &sender = Event["sender"];
    btype.user_id = to_string(Event["user_id"]);
    user_name = btype.ifgroup ? sender["card"] : sender["nickname"];
    if (!user_name.length())
      user_name = sender["nickname"];

    if (btype.ifgroup)
    {

      if (sender["role"] == "owner")
        admin_level += 20;
      else if (sender["role"] == "admin")
        admin_level += 10;

      btype.group_id = to_string(Event["group_id"]);
    }
    else
    {
      btype.target_id = to_string(Event["target_id"]);
    }
  }
  else if (post_type == "notice")
  {
    json &ntype = Event["notice_type"];
    if (ntype == "friend_recall" || ntype == "group_recal")
    {
      btype.message = "[recall]";
      btype.user_id = to_string(Event["user_id"]);
      if (ntype == "group_recall")
      {
        btype.ifgroup = 1;
        btype.group_id = to_string(Event["group_id"]);
      }
    }
    else
    {
      if (Event.count("group_id"))
      {
        btype.ifgroup = 1;
        btype.group_id = to_string(Event["group_id"]);
      }
      if (Event.count("user_id"))
        btype.user_id = to_string(Event["user_id"]);
      // btype.message = ntype;
      if (ntype == "notify")
      {
        if (Event["sub_type"] == "poke")
        {
          btype.message = (to_string(Event["target_id"]) == configs.Self_ID)
                              ? "[poke]"
                              : "[poke_others]";
        }
        else if (Event["sub_type"] == "honor")
          btype.message = "[honor_"s + string(Event["honor_type"]) + "]";
        else
          btype.message = Event["sub_type"];
      }
      else
        btype.message = "["s + string(ntype) + "]";
    }
  }
  else if (post_type == "request")
  {
    btype.message = "[request]\n"s + "[type]=" + to_string(Event["request_type"]) + "\n[flag]=" + to_string(Event["flag"]);
  }
  else
    btype.message = "[INVALID EVENT]";

  if (btype.user_id.length() < 1)
    btype.user_id = "10000";

  admin_level += configs.list_config.get_admin_level(btype.user_id);
  get_name();
  show();
}

#endif
