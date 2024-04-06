#include "Jiemeng.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Deck.hpp"
using Type = Operation::Type;
string Jiemeng::exec_operation(Message &message, const Operation &operation)
{
  if (operation.type == Type::message)
    return (operation.str);
  if (operation.type == Type::lua_call)
  {
    lua->call(operation.str, message);
    return "";
  }
  if (operation.type == Type::lua_shell)
    return (lua->exec(operation.str));
  if (operation.type == Type::draw_deck)
    return (deck->draw(operation.str));
  if(operation.type==Type::clear)
    throw Operation::Clear();
  throw invalid_argument("未知的 Operation");
}