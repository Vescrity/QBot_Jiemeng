#ifndef MESSAGE_OUTPUT
#define MESSAGE_OUTPUT
/**
 *      _ _
 *     | (_) ___ _ __ ___   ___ _ __   __ _
 * _   | | |/ _ \ '_ ` _ \ / _ \ '_ \ / _` |
 * | |_| | |  __/ | | | | |  __/ | | | (_| |
 *  \___/|_|\___|_| |_| |_|\___|_| |_|\__, |
 *                                    |___/
 */
#include "Message_Type.h"
#include "txt2img_api.h"
#include "Bot_Send_Check.h"
#include "Jiemeng_Config_Class.h"
#include "Time_Struct.h"
#include "Orders.h"
#include "r_adb.h"
#include "urls.h"
#include "CQjson.h"
#include <string>
/// @brief Send the message given without any change
/// @param data Message type
/// @param msg The message need to send
/// @return a JSON with information about the details on the server response.
json Basic_Message_Output(const Basic_Message_Type &data, const string &msg)
{
  json send_data;
  try
  {
    if (data.ifgroup)
    {
      send_data["params"]["group_id"] = stoi(data.group_id);
      send_data["action"] = "send_group_msg";
    }
    else
    {
      send_data["params"]["user_id"] = stoi(data.user_id);
      send_data["action"] = "send_private_msg";
    }
  }
  catch (...)
  {
  }

  try
  {
    send_data["params"]["message"] = CQ2json(msg);
  }
  catch (...)
  {
    send_data["message"] = "Code Error";
  }
  return ws_json_send(send_data);
  /*rq->set_url("http://127.0.0.1:"s + num2str(configs.OUTPORT));
  rq->set_data(send_data);
  string rt = rq->Post();
  delete rq;
  return rt;*/
}
/// @brief Send the message given without any change
/// @param data Message type contains the message need to send
/// @return a JSON with information about the details on the server response.
json Basic_Message_Output(const Basic_Message_Type &data)
{
  return Basic_Message_Output(data, data.message);
}
/// @brief Try to send the given message
/// @param data Message type
/// @param msg Message need to send
/// @return 1
bool Message_Output(const Basic_Message_Type &data, const string &msg)
{
  int &MAX_MESSAGE_LENGTH = configs.MAX_MESSAGE_LENGTH;
  int &MAX_SINGLE_MESSAGE_LENGTH = configs.MAX_SINGLE_MESSAGE_LENGTH;
  if (msg.length() > MAX_MESSAGE_LENGTH)
  {
    string num1, num2;
    num1 = to_string(MAX_MESSAGE_LENGTH);
    num2 = to_string(msg.length());
    throw std::runtime_error("Message TOO LONG: \n  MAX="s + num1 + "\n  Message=" + num2);
  }

  size_t pos = msg.find("[-cut-]");
  if (pos != string::npos)
  {
    Message_Output(data, msg.substr(0, pos));
    return Message_Output(data, msg.substr(pos + strlen("[-cut-]")));
  }
  string str, tmp;
  if (msg.length() > configs.MAX_TEXT_LENGTH)
  {
    Basic_Message_Output(data, txt2img(msg.c_str()));
  }
  else
    for (int i = 0; i < msg.size(); i += MAX_SINGLE_MESSAGE_LENGTH)
    {
      if (!msg.length())
        return 1;
      tmp = msg.substr(i, MAX_SINGLE_MESSAGE_LENGTH);
      /*if (tmp.length() > configs.MAX_TEXT_LENGTH)
      {
        Basic_Message_Output(data, txt2img(tmp.c_str()));
        minisleep(configs.delay_time);
        continue;
      }*/

      json js = Basic_Message_Output(data, tmp);
      if (js["status"] == "ok")
      {
        bot_check.in(js["data"]["message_id"]);
        info_lable("[Output]");
        info_puts("Send OK!");
      }
      else
      {
        error_lable("[Output]");
        error_puts("It might be failed.");
        /*error_puts(string(js["wording"]).c_str());
        // 发送失败操作
        if (string(js["wording"]).find("blocked by server") != string::npos)*/
        //  Basic_Message_Output(data, txt2img(tmp.c_str()));
      }
      minisleep(configs.delay_time);
    }
  return 1;
}
/// @brief Try to send the given message
/// @param data Message type contains the message need to send
/// @return 1
bool Message_Output(const Basic_Message_Type &data)
{
  return Message_Output(data, data.message);
}
/// @brief Change the given message into a After_change message
/// @param type Message type
/// @param msg given message
/// @return After_change message
string Basic_Message_Change(const Message_Type &type, const string &msg)
{
  string nmsg = msg;
  nmsg = Basic_String_Replace("{%", "}", nmsg, [&type](const string &str) -> string
                              { return reRecv(type, str); });
  nmsg = str_strchg("[Repeat]", type.btype.message.c_str(), nmsg.c_str());
  nmsg = str_strchg("[name]", type.user_name.c_str(), nmsg.c_str());
  nmsg = str_strchg("[group_name]", type.group_name.c_str(), nmsg.c_str());
  nmsg = str_strchg("[group_id]", type.btype.group_id.c_str(), nmsg.c_str());
  nmsg = str_strchg("[user_id]", type.btype.user_id.c_str(), nmsg.c_str());
  nmsg = str_strchg("[\\n]", "\n", nmsg.c_str());
  str_tm *stm = new str_tm;
  nmsg = str_strchg("[year]", stm->year, nmsg.c_str());
  nmsg = str_strchg("[month]", stm->month, nmsg.c_str());
  nmsg = str_strchg("[yday]", stm->yday, nmsg.c_str());
  nmsg = str_strchg("[mday]", stm->mday, nmsg.c_str());
  nmsg = str_strchg("[wday]", stm->wday, nmsg.c_str());
  nmsg = str_strchg("[hour]", stm->hour, nmsg.c_str());
  nmsg = str_strchg("[min]", stm->min, nmsg.c_str());
  nmsg = str_strchg("[sec]", stm->sec, nmsg.c_str());
  delete stm;
  nmsg = chg_Rcode(nmsg.c_str());
  return nmsg;
}
/// @brief Output the given message(Unformatted).
/// @param type Message type
/// @param msg given message
/// @return send ok?
bool Standard_Message_Output(const Message_Type &type, const string &msg)
{
  if (!msg.length())
    return 1;
  string strs;
  if (type.btype.admin_level > -1)
    strs = Basic_Message_Change(type, msg);
  else
    strs = msg;
  color_lable("[Output]");
  color_puts(strs.c_str());
  try
  {
    Message_Output(type.btype, strs);
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Output]")
    return 0;
  }
  return 1;
}
#endif