#include "Jiemeng.hpp"

bool Jiemeng::message_output(const string &user_id, CQMessage &message)
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
bool Jiemeng::message_output(const string &group_id, const string &user_id, CQMessage &message)
{
  json send_data, rt;
  send_data["action"] = "send_group_msg";
  send_data["params"]["user_id"] = stoi(user_id);
  send_data["params"]["group_id"] = stoi(group_id);
  send_data["params"]["message"] = message.get_json();
  rt = ws_send(send_data);
  debug_lable("[Output]");
  dout << rt << "\n";
  return rt["status"] == "ok";
}
bool Jiemeng::message_output(const Message_Place &place, CQMessage &message)
{
  if (place.is_group())
    return message_output(place.group_id, place.user_id, message);
  return message_output(place.user_id, message);
}
bool Jiemeng::message_output(Message &msg)
{
  return message_output(msg.place,msg.message());
}
