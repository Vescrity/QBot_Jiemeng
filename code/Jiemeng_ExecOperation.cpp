#include "Jiemeng.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Deck.hpp"
using Type = Operation::Type;
void Jiemeng::exec_operation(Message &message, const Operation &operation)
{
  if (operation.type == Type::message)
  {
    CQMessage ms(operation.str);
    message_output(message.place, ms);
  }
  else if (operation.type == Type::lua_call)
  {
    lua->call(operation.str, message);
  }
  else if (operation.type == Type::lua_shell)
  {
    CQMessage ms(lua->exec(operation.str));
    message_output(message.place, ms);
  }else if (operation.type == Type::draw_deck)
  {
    CQMessage ms(deck->draw(operation.str));
    message_output(message.place, ms);
  }
  
}