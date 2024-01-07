#ifndef ORDERS
#define ORDERS
/**
 *      _ _
 *     | (_) ___ _ __ ___   ___ _ __   __ _
 * _   | | |/ _ \ '_ ` _ \ / _ \ '_ \ / _` |
 * | |_| | |  __/ | | | | |  __/ | | | (_| |
 *  \___/|_|\___|_| |_| |_|\___|_| |_|\__, |
 *                                    |___/
 */
#include "url_order.h"
#include "txt2img_api.h"
#include "Jiemeng_Plugin.h"
#include "Baidu_Trans_API.h"
#include "Message_Delete.h"
#include "Weather_Report.h"
string reRecv(const Message_Type &type, const string &msg);
void type_reRecv(Basic_Message_Type &type);
void type_reRecv(Basic_Message_Type &type, const string &msg);
void type_rePost(Basic_Message_Type &type);
void type_rePost(Basic_Message_Type &type, const string &msg);
//------------------------------------------------------------
// str_Message_Operate


string str_Message_Operate(const Message_Type &type);
void Message_Operate(const Message_Type &type);

string reRecv(const Message_Type &type, const string &msg)
{
  Message_Type t = type;
  t.btype.message = msg;
  return str_Message_Operate(t);
}
void type_reRecv(Basic_Message_Type &type)
{
  if (type.group_id == "WHITE_LIST")
  {
    for (auto &i : configs.list_config.group_list)
    {
      type.group_id = i;
      type_reRecv(type);
    }
    return;
  }
  else if (!type.ifgroup)
  {
    if (type.group_id == "BROAD_LIST")
    {
      for (auto &i : configs.list_config.broad_list)
      {
        type.user_id = i;
        type_reRecv(type);
      }
      return;
    }
  }
  Message_Type mtype;
  mtype.btype = type;
  mtype.get_name();
  return Message_Operate(mtype);
}
void type_reRecv(Basic_Message_Type &type, const string &msg)
{
  type.message = msg;
  return type_reRecv(type);
}
void type_rePost(Basic_Message_Type &type)
{
  type_rePost(type, type.message);
}
bool Standard_Message_Output(const Message_Type &type, const string &msg);
void type_rePost(Basic_Message_Type &type, const string &msg)
{
  if (type.group_id == "WHITE_LIST")
  {
    for (auto &i : configs.list_config.group_list)
    {
      type.group_id = i;
      type_rePost(type,msg);
    }
    return;
  }
  else if (!type.ifgroup)
  {
    if (type.group_id == "BROAD_LIST")
    {
      for (auto &i : configs.list_config.broad_list)
      {
        type.user_id = i;
        type_rePost(type,msg);
      }
      return;
    }
  }
  Message_Type mtype;
  mtype.btype = type;
  mtype.get_name();
  Standard_Message_Output(mtype, msg);
}

#endif