#ifndef ORDERS
#define ORDERS
// 参数分割
void Get_Para(const std::string &str, int n, std::vector<std::string> *result);
#include "url_order.h"
#include "txt2img_api.h"
#include "Jiemeng_Plugin.h"
string resend(const Message_Type &type, const string &msg);
void type_resend(Basic_Message_Type &type);
void type_resend(Basic_Message_Type &type, const string &msg);
void type_repost(Basic_Message_Type &type);
void type_repost(Basic_Message_Type &type, const string &msg);
//------------------------------------------------------------
// str_Message_Operate
string str_Message_Operate(const Message_Type &type);
void Message_Operate(const Message_Type &type);

string resend(const Message_Type &type, const string &msg)
{
  Message_Type t = type;
  t.btype.message = msg;
  return str_Message_Operate(t);
}
void type_resend(Basic_Message_Type &type)
{
  if (type.group_id == "WHITE_LIST")
  {
    for (auto &i : configs.list_config.group_list)
    {
      type.group_id = i;
      type_resend(type);
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
        type_resend(type);
      }
      return;
    }
  }
  Message_Type mtype;
  mtype.btype = type;
  mtype.get_name();
  return Message_Operate(mtype);
}
void type_resend(Basic_Message_Type &type, const string &msg)
{
  type.message = msg;
  return type_resend(type);
}
void type_repost(Basic_Message_Type &type)
{
  type_repost(type, type.message);
}
void type_repost(Basic_Message_Type &type, const string &msg)
{
  Message_Type mtype;
  mtype.btype = type;
  mtype.get_name();
  Standard_Message_Output(mtype, msg);
}
void Get_Para(const std::string &str, int n, std::vector<std::string> *result)
{
  result->clear();
  size_t pos = 0;
  for (int i = 0; i < n - 1; ++i)
  {
    size_t nextPos = str.find("#", pos);
    if (nextPos == std::string::npos)
    {
      throw std::runtime_error("The string can't cut to n parts");
    }
    result->push_back(str.substr(pos, nextPos - pos));
    pos = nextPos + 1;
  }
  if (pos >= str.size())
  {
    throw std::runtime_error("The string can't cut to n parts");
  }
  result->push_back(str.substr(pos));
}
#endif