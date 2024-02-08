#ifndef MESSAGE_OPERATE
#define MESSAGE_OPERATE

#include "Message_Type.h"
#include "Jiemeng_Ans.h"
#include "Message_Output.h"
#include "Pre_Catch.h"
#include "Jiemeng_Order.h"
string str_Oper_Explain(const Message_Type &type, const Oper_List &oplist, bool &ignore);
void Oper_Explain(const Message_Type &, const Oper_List &, bool &);
bool Pre_Catch_List::Operate(const Message_Type &type)
{
  for (auto it = pre_catch.begin(); it < pre_catch.end(); it++)
  {
    auto &i = *it;
    if (i->Check(type.btype))
    {
      bool ignore = 0;
      Oper_List ol = i->anss.get_val();
      if (!i->survive)
        pre_catch.erase(it);
      Oper_Explain(type, ol, ignore);
      if (ignore)
        continue;
      else
        return 1;
    }
  }
  return 0;
}
/// @brief Operate the given message and return the left message un-outputed.
/// @param type The type with message.
/// @return The left message.
string str_Message_Operate(const Message_Type &type)
{
  // 指令检查
  string rt;
  if (pre_catch_list.Operate(type))
    return "";
  // 词库匹配
  for (auto i : answers.ans_group)
  {
    if (i->check(type.btype))
    {
      bool ignore = 0;
      rt += str_Oper_Explain(type, i->anss.get_val(), ignore);
      if (ignore)
        continue;
      else
        return rt;
    }
  }
  return rt;
}
void Message_Operate(const Message_Type &type)
{
  // 指令检查

  if (pre_catch_list.Operate(type))
    return;
  // 词库匹配
  for (auto i : answers.ans_group)
  {
    if (i->check(type.btype))
    {
      bool ignore = 0;
      Oper_Explain(type, i->anss.get_val(), ignore);
      if (ignore)
        continue;
      else
        return;
    }
  }
}
string str_Oper_Explain(const Message_Type &type, const Oper_List &oplist, bool &ignore)
{
  using Oper = Answer_Oper::Oper;
  string strs;
  for (auto &i : oplist.list)
  {
    if (i.oper == Oper::output)
    {
      if (i.str != "[IGNORE]")
        strs += i.str;
      else
        ignore = 1;
    }
    else if (i.oper == Oper::order)
    {
      string tmp;
      if (i.ifjs)
        tmp = Order(type, i.str, i.js);
      else
      {
        const string paras = i.msgread ? type.btype.message.substr(type.btype.message.find("#") + 1) : i.str.substr(i.str.find("#") + 1);
        const string orders = i.msgread
                                  ? i.str
                                  : i.str.substr(0, i.str.find("#"));;
        tmp = Order(type, orders, paras);
      }
      if (tmp == "[-clear-]")
      {
        Standard_Message_Output(type, strs);
        strs = "";
      }
      else if (tmp == "[IGNORE]")
        ignore = 1;
      else
        strs += tmp;
    }
  }
  return strs;
}
inline void Oper_Explain(const Message_Type &type, const Oper_List &oplist, bool &ignore)
{
  Standard_Message_Output(type, str_Oper_Explain(type, oplist, ignore));
}

#endif