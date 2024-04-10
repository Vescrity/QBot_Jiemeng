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
  if (oper.type == Type::ignore)
  {
    return rt;
  }
  if (oper.type == Type::order)
  {
    auto para = string_cut(oper.str, bot->config.spliter, 2);
    auto &paras = (*para);
    dout << "PARA0" << paras[0] << "\n";
    string &order = paras[0];
    if (order == "order")
    {
      auto Pa = string_cut(message.true_str(), bot->config.spliter, 2);
      oper.str = (*Pa)[1];
      rt += extract(oper, message, bot);
      return rt;
    }
    if (order == "s_sh")
    {
      auto Pa = string_cut(paras[1], bot->config.spliter, 2);
      auto &state = (*Pa)[0];
      auto &code = (*Pa)[1];
      oper.str = bot->map_lua[state]->exec(code);
      oper.type = Type::message;
      rt += oper;
      return rt;
    }
    if (order == "l_sh")
    {
      auto &Pa = paras[1];
      oper.str = bot->lua->exec(Pa);
      oper.type = Type::message;
      rt += oper;
      return rt;
    }
    if (order == "1_sh")
    {
      auto &Pa = paras[1];
      Lua_Shell *l = new Lua_Shell(bot);
      oper.str = l->exec(Pa);
      delete l;
      oper.type = Type::message;
      rt += oper;
      return rt;
    }
    if (order == "sh")
    {
      auto &Pa = paras[1];
      oper.str = execmd(Pa);
      oper.type = Type::message;
      rt += oper;
      return rt;
    }

    if (order == "draw_deck")
    {
      oper.type = Type::draw_deck;
      oper.str = paras[1];
      rt += extract(oper, message, bot);
      return rt;
    }
    if (order == "deck_reload")
    {
      bot->deck_reload();
      return rt;
    }
    if (order == "answer_reload")
    {
      bot->answer_reload();
      return rt;
    }
    if (order == "reRecv")
    {
      message.str() = paras[1];
      rt = bot->answer.get_list(message);
      rt.upgrade(message, bot);
      return rt;
    }
    if (order == "type_rePost")
    {
      dout << "paras1" << paras[1] << "\n";
      auto Para = string_cut(paras[1], bot->config.spliter, 3);
      auto &Paras = *Para;
      message_replace(Paras[2], message.place);
      str_replace(Paras[2], "[Repeat]", message.text.const_str());
      Operation op;
      op.str = Paras[2];
      op.type = Type::message;
      Message msg;
      msg.change("");
      if (Paras[0] != "0")
      {
        msg.place.set_group();
        msg.place.group_id = Paras[0];
      }
      else
      {
        msg.place.set_private();
        msg.place.user_id = Paras[1];
      }
      Operation_List opl;
      opl += op;
      bot->process_operation(msg, opl);
      return rt;
    }
  }
  debug_lable("[Extract]");
  dout << "发现了未被 extract 解析的 operation.\n";
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