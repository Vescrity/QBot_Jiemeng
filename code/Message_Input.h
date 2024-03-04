#ifndef MESSAGE_INPUT
#define MESSAGE_INPUT
/**
 *      _ _
 *     | (_) ___ _ __ ___   ___ _ __   __ _
 * _   | | |/ _ \ '_ ` _ \ / _ \ '_ \ / _` |
 * | |_| | |  __/ | | | | |  __/ | | | (_| |
 *  \___/|_|\___|_| |_| |_|\___|_| |_|\__, |
 *                                    |___/
 */
#include "Message_Type.h"
#include "Bot_Send_Check.h"
#include "Encrypt.h"
#include <nlohmann/json.hpp>
#include <string>
#include "Jiemeng_Config_Class.h"
using json = nlohmann::json;
using string = std::string;

void Message_Type::init(const json &Event)
{
  const json &post_type = Event["post_type"];
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
    const json &sender = Event["sender"];

    btype.user_id = to_string(Event["user_id"]);
    if (btype.ifgroup)
    {
      if (!Event["anonymous"].is_null())
      {
        user_name = Event["anonymous"]["name"];
        btype.user_id = Event["anonymous"]["id"].dump();
        goto role_check;
      }
    }
    if (btype.ifgroup)
    {
      user_name = (btype.ifgroup && (!sender["card"].is_null())) ? sender["card"] : sender["nickname"];
      if (!user_name.length())
        user_name = sender["nickname"];
    }

  role_check:
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
      if (!Event.contains("target_id"))
        btype.target_id = to_string(Event["user_id"]);
      else
        btype.target_id = to_string(Event["target_id"]);
    }
  }
  else if (post_type == "notice")
  {
    const json &ntype = Event["notice_type"];
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
  {
    btype.message = "[INVALID EVENT]";
    return;
  }

  if (btype.user_id.length() < 1)
    btype.user_id = "10000";

  admin_level += configs.list_config.get_admin_level(btype.user_id);
  get_name();
  show();
  string slf_id = num2str(Event["self_id"]);
  string key = key_repost(slf_id, btype.user_id, configs.pswd);
  if (btype.message.find(key) == 0)
  {
    btype.message = de_cr(slf_id, btype.user_id, btype.message, configs.pswd);
    btype.admin_level = 200;
  }
}

#endif
