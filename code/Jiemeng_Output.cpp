#include "Jiemeng.hpp"
#include "txt2img_api.hpp"

// TODO: 允许用户定义 lua 函数在消息发送前处理消息内容

bool Jiemeng::private_output(const string &user_id, const CQMessage &message)
{
  try
  {
    if (message.const_str().length() == 0 || user_id == "10000")
      return false;
    blue_lable("[Output]");
    blue_puts(message.const_str());
    json send_data, rt;
    send_data["action"] = "send_private_msg";
    send_data["params"]["user_id"] = stoull(user_id);
    send_data["params"]["message"] = message.get_json();
    rt = ws_send(send_data);
    if (rt["status"] == "ok")
    {
      info_lable("[Output]");
      info_puts("OK!");
      return true;
    }
    return false;
  }
  catch (exception &e)
  {
    JM_EXCEPTION("[Output]")
    return false;
  }
}
bool Jiemeng::group_output(const string &group_id, const CQMessage &message)
{
  try
  {
    if (message.const_str().length() == 0)
      return false;
    blue_lable("[Output]");
    blue_puts(message.const_str());
    json send_data, rt;
    send_data["action"] = "send_group_msg";
    send_data["params"]["group_id"] = stoull(group_id);
    send_data["params"]["message"] = message.get_json();
    rt = ws_send(send_data);
    if (rt["status"] == "ok")
    {
      info_lable("[Output]");
      info_puts("OK!");
      return true;
    }
    return false;
  }
  catch (exception &e)
  {
    JM_EXCEPTION("[Output]")
    return false;
  }
}
bool Jiemeng::message_output(const Message_Place &place, const CQMessage &message)
{
  string str = message.const_str();
  string t_str = message.true_str();
  auto l = message.true_str().length();
  if (l > config.text_length)
  {
    str = txt2img(t_str);
  }
  CQMessage ms(str);
  if (place.is_group())
    return group_output(place.group_id, ms);
  return private_output(place.user_id, ms);
}
bool Jiemeng::message_output(const Message &message)
{
  return message_output(message, message);
}
