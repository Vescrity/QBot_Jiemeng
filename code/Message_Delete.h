#ifndef MESSAGE_DELETE
#define MESSAGE_DELETE
/**
 *      _ _
 *     | (_) ___ _ __ ___   ___ _ __   __ _
 * _   | | |/ _ \ '_ ` _ \ / _ \ '_ \ / _` |
 * | |_| | |  __/ | | | | |  __/ | | | (_| |
 *  \___/|_|\___|_| |_| |_|\___|_| |_|\__, |
 *                                    |___/
 */
#include "Jiemeng_Exception.h"
#include "Jiemeng_Config_Class.h"
void Message_Delete(const string &msid)
{
  try
  {
    json js;
    js["params"]["message_id"] = msid;
    js["action"] = "delete_msg";
    ws_json_send(js);
    return;
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Delete]")
    return;
  }
}
void Message_Delete_Order(const string &order)
{
  auto l = order.find("reply,id=");
  if (l == string::npos)
    throw invalid_argument("Invalid Message_Delete Order");
  auto r = order.substr(l).find("]");
  r += l;
  l += 9;
  Message_Delete(order.substr(l, r - l));
}

#endif