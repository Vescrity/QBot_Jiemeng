#ifndef MESSAGE_TYPE
#define MESSAGE_TYPE
#include "Jiemeng_Basic.h"
/**
 *      _ _
 *     | (_) ___ _ __ ___   ___ _ __   __ _
 * _   | | |/ _ \ '_ ` _ \ / _ \ '_ \ / _` |
 * | |_| | |  __/ | | | | |  __/ | | | (_| |
 *  \___/|_|\___|_| |_| |_|\___|_| |_|\__, |
 *                                    |___/
 */
#include <string>
using string = std::string;
class Basic_Message_Type
{
public:
  std::string user_id, group_id, target_id, message;
  int admin_level;
  bool ifgroup;
  Basic_Message_Type() { admin_level = 0; }
  bool same(const Basic_Message_Type &type) const
  {
    if (ifgroup != type.ifgroup)
      return 0;
    else if (ifgroup)
      return group_id == type.group_id && user_id == type.user_id;
    else
      return user_id == type.user_id;
  }
  bool is_white();
};
class Message_Type
{
public:
  Basic_Message_Type btype;
  string user_name, group_name;
  bool white;
  Message_Type() { white = 1; }
  void get_name();
  void init(const string &);
  void show() const;
};
/// @brief Print the info of the message
void Message_Type::show() const
{
  string cutl = "\n-------------------"s + JIEMENG_PLATFORM + " " + JIEMENG_VERSION + "-------------------";
  color_puts(cutl.c_str());
  if (btype.ifgroup)
  {
    info_lable("[group_id]");
    info_puts(btype.group_id.c_str());
    info_lable("[group_nm]");
    info_puts(group_name.c_str());
  }
  info_lable("[user_id]");
  info_puts(btype.user_id.c_str());
  info_lable("[user_nm]");
  info_puts(user_name.c_str());
  msg_lable("[message]");
  msg_puts(btype.message.c_str());
}
#ifdef JIEMENG_CONFIG
#ifdef URLS
/// @brief Get the user's name of the message
void Message_Type::get_name()
{
  if (btype.user_id == "ADMIN")
    return;
  try
  {
    if (btype.ifgroup && group_name.length() == 0)
    {
      json js;
      js["group_id"] = btype.group_id;
      Request *rq = new Request;
      rq->set_api("/get_group_info");
      rq->set_url("http://127.0.0.1:"s + num2str(configs.OUTPORT));
      rq->set_data(js);
      js = rq->js_post();
      delete rq;
      group_name = js["data"]["group_name"];
    }
    if (user_name.length() == 0)
    {
      json js;
      js["user_id"] = btype.user_id;
      Request *rq = new Request;
      rq->set_api("/get_stranger_info");
      rq->set_url("http://127.0.0.1:"s + num2str(configs.OUTPORT));
      rq->set_data(js);
      js = rq->js_post();
      delete rq;
      user_name = js["data"]["nickname"];
    }
  }
  catch (exception &e)
  {
    std::string msg = "Exception caught: ";
    msg += e.what();
    error_lable("[get_name]");
    error_puts(msg.c_str());
  }
}

#endif

bool Basic_Message_Type::is_white()
{
  if (admin_level > 200)
    return 1;
  List_Config &lst = configs.list_config;
  if (lst.is_black(user_id))
    return 0;
  if (ifgroup)
    return lst.group_white(group_id);
  else
    return lst.private_white(user_id);
}
#endif
#endif