#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Random.hpp"
#include "Jiemeng_Time.hpp"
#include "Jiemeng_MessageReplace.hpp"
using Type = Operation::Type;
Operation_List extract(Operation &oper, Message &message, Jiemeng *bot)
{
  Operation_List rt;
  if (oper.type == Type::message)
  {
    message_replace(oper.str, message.place);
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

void Operation::set_type(const string &t)
{
  if (t == "message")
  {
    type = Type::message;
    return;
  }
  if (t == "order")
  {
    type = Type::order;
    return;
  }
  if (t == "lua_call")
  {
    type = Type::lua_call;
    return;
  }
  if (t == "lua_shell")
  {
    type = Type::lua_shell;
    return;
  }
  if (t == "ignore")
  {
    type = Type::ignore;
    return;
  }
  if (t == "draw_deck")
  {
    type = Type::draw_deck;
    return;
  }
  if (t == "clear")
  {
    type = Type::clear;
    return;
  }
  throw invalid_argument("未定义的 Operation_Type");
}