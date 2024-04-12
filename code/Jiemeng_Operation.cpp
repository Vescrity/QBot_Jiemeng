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

Operation_List extract(Operation oper, Message message, Jiemeng *bot)
{
  Operation_List rt;
  Operation clears;
  clears.type = Type::clear;
  if (oper.type == Type::message)
  {
    message_replace(oper.str, message.place);
    str_replace(oper.str, "[Repeat]", message.text.const_str());
    auto uwu = string_cut(oper.str, "[-cut-]");
    Operation op;
    op.type = Type::message;
    int qwq = 0;
    for (auto &i : *uwu)
    {
      op.str = i;
      rt += op;
      if (qwq++)
        rt += clears;
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
  if (oper.type == Type::ignore)
  {
    return rt;
  }
  if (oper.type == Type::order)
  {
    auto para = string_cut(oper.str, bot->config.spliter, 2);
    auto &paras = (*para);
    string &order = paras[0];
    if (order == "order")
    {
      auto Pa = string_cut(message.true_str(), bot->config.spliter, 2);
      oper.str = (*Pa)[1];
      rt += extract(oper, message, bot);
      return rt;
    }
    if (order == "sleep")
    {
      auto Pa = string_cut(message.true_str(), bot->config.spliter, 2);
      oper.data = stoi((*Pa)[1]);
      return rt;
    }

    if (order == "draw_deck")
    {
      oper.type = Type::draw_deck;
      oper.str = paras[1];
      rt += extract(oper, message, bot);
      return rt;
    }
    if (order == "reRecv")
    {
      message.str() = paras[1];
      rt = bot->answer.get_list(message);
      rt.upgrade(message, bot);
      return rt;
    }
    rt += oper;
    return rt;
  }
  rt += oper;
  return rt;
}

void Operation_List::upgrade(Message &message, Jiemeng *bot)
{
  debug_lable("[Upgrade]");
  dout << "开始对 Operation_List 执行更新操作\n";
  Operation_List rt;
  for (auto &i : list)
    rt += extract(i, message, bot);
  /*Operation c;
  c.type = Type::clear;
  rt += c;
  list.clear();*/
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
  if (t == "lua_once")
  {
    type = Type::lua_once;
    return;
  }
  if (t == "call_state")
  {
    type = Type::call_state;
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

Operation::Operation()
{
  type = Type::message;
  str = "clear";
}