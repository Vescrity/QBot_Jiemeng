#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Random.hpp"
#include "Jiemeng_Time.hpp"
using Type = Operation::Type;
Operation_List extract(Operation &oper, Message &message, Jiemeng *bot)
{
  Operation_List rt;
  if (oper.type == Type::message)
  {
    oper.str = chg_Rcode(oper.str);
    Time_Class tm;
    str_replace(oper.str, "[year]", tm.get_year());
    str_replace(oper.str, "[month]", tm.get_month());
    str_replace(oper.str, "[yday]", tm.get_yearday());
    str_replace(oper.str, "[mday]", tm.get_monthday());
    str_replace(oper.str, "[wday]", tm.get_weekday());
    str_replace(oper.str, "[hour]", tm.get_hour());
    str_replace(oper.str, "[min]", tm.get_min());
    str_replace(oper.str, "[sec]", tm.get_sec());
    str_replace(oper.str, "[group_name]", message.place.group_nm);
    str_replace(oper.str, "[group_id]", message.place.group_id);
    str_replace(oper.str, "[user_name]", message.place.user_nm);
    str_replace(oper.str, "[user_id]", message.place.user_id);
    str_replace(oper.str, "[Repeat]", message.text.const_str());
    auto uwu = string_cut(oper.str, "[-cut-]");
    Operation op;
    op.type = Type::message;
    for (auto i : *uwu)
    {
      op.str = i;
      rt += op;
    }
    return rt;
  }
  if (oper.type == Type::draw_deck)
  {
    oper.str = bot->deck->draw(oper.str);
    oper.type = Type::message;
    rt += extract(oper, message, bot);
    return rt;
  }
  rt += oper;
  return rt;
}

void Operation_List::upgrade(Message &message, Jiemeng *bot)
{
  Operation_List rt;
  for (auto &i : list)
    rt += extract(i, message, bot);
  Operation c;
  c.type = Type::clear;
  rt += c;
  list.clear();
  list = rt.list;
}
