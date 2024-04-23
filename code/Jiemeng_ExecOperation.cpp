#include "Jiemeng.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_MessageReplace.hpp"
using Type = Operation::Type;
string Jiemeng::exec_operation(const Message &message, const Operation &operation)
{
  dout << "OPER:" << int(operation.type) << "\n";
  dout << "OPER:" << operation.str << "\n";
  if (operation.type == Type::message)
    return (operation.str);
  if (operation.type == Type::sleep)
  {
    minisleep(int(operation.data));
    return "";
  }
  if (operation.type == Type::lua_call)
  {
    return lua->call(operation.str, message);
  }
  if (operation.type == Type::lua_once)
  {
    auto one = new Lua_Shell(this);
    string rt = one->call(operation.str, message);
    delete one;
    return rt;
  }
  if (operation.type == Type::call_state)
  {
    auto s = map_lua[operation.str];
    return s->call(operation.data["func"], message);
  }
  if (operation.type == Type::lua_shell)
    return (lua->exec(operation.str));

  if (operation.type == Type::order)
  {
    Operation oper = operation;
    auto para = string_cut(oper.str, this->config.spliter, 2);
    auto &paras = (*para);
    string &order = paras[0];
    if (order == "s_sh")
    {
      auto Pa = string_cut(paras[1], this->config.spliter, 2);
      auto &state = (*Pa)[0];
      auto &code = (*Pa)[1];
      return this->map_lua[state]->exec(code);
    }
    if (order == "l_sh")
    {
      auto &Pa = paras[1];
      return this->lua->exec(Pa);
    }
    if (order == "1_sh")
    {
      auto &Pa = paras[1];
      Lua_Shell *l = new Lua_Shell(this);
      auto r = l->exec(Pa);
      delete l;
      return r;
    }
    if (order == "sh")
    {
      auto &Pa = paras[1];
      return execmd(Pa);
    }
    if (order == "answer_reload")
    {
      answer_reload();
      return "";
    }
    if (order == "deck_reload")
    {
      deck_reload();
      return "";
    }
    if (order == "type_rePost")
    {
      dout << "paras1" << paras[1] << "\n";
      auto Para = string_cut(paras[1], config.spliter, 3);
      auto &Paras = *Para;
      message_replace(Paras[2], message);
      Operation op;
      op.str = Paras[2];
      op.type = Type::message;
      Message msg;
      msg.change("");
      if (Paras[0] != "0")
      {
        msg.set_group();
        msg.group_id = Paras[0];
      }
      else
      {
        msg.set_private();
        msg.user_id = Paras[1];
      }
      Operation_List opl;
      opl += op;
      process_operation(msg, opl);
      return "";
    }
    if (order == "reRecv")
    {
      throw Operation::reRecv(paras[1]);
    }
  }
  // 理论上不应该出现draw_deck
  if (operation.type == Type::draw_deck)
    throw runtime_error("BUG: draw_deck 不应出现在这里");
  if (operation.type == Type::clear)
    throw Operation::Clear();
  throw invalid_argument("未知的 Operation");
}