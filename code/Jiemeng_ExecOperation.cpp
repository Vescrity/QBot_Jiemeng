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
    return lua->call(operation.str, message);
  }
  if (operation.type == Type::lua_shell)
    return (lua->exec(operation.str));
  // 理论上不应该出现draw_deck
  if (operation.type == Type::draw_deck)
    throw runtime_error("BUG: draw_deck 不应出现在这里");
  if(operation.type==Type::clear)
    throw Operation::Clear();
  throw invalid_argument("未知的 Operation");
}