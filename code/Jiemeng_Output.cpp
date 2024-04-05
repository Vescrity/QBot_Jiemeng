#include "Jiemeng.hpp"
#include "txt2img_api.hpp"
bool Jiemeng::private_output(const string &user_id, CQMessage &message)
{
  json send_data, rt;
  send_data["action"] = "send_private_msg";
  send_data["params"]["user_id"] = stoi(user_id);
  send_data["params"]["message"] = message.get_json();
  rt = ws_send(send_data);
  debug_lable("[Output]");
  dout << rt << "\n";
  return rt["status"] == "ok";
}
bool Jiemeng::group_output(const string &group_id, CQMessage &message)
{
  json send_data, rt;
  send_data["action"] = "send_group_msg";
  send_data["params"]["group_id"] = stoi(group_id);
  send_data["params"]["message"] = message.get_json();
  rt = ws_send(send_data);
  debug_lable("[Output]");
  dout << rt << "\n";
  return rt["status"] == "ok";
}
bool Jiemeng::message_output(const Message_Place &place, CQMessage &message)
{
  string str = message.str();
  if (str.length() > config.text_length)
  {
    str = txt2img(str);
  }
  CQMessage ms(str);
  if (place.is_group())
    return group_output(place.group_id, ms);
  return private_output(place.user_id, ms);
}
bool Jiemeng::message_output(Message &msg)
{
  return message_output(msg.place, msg.message());
}
