#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Message.hpp"
using Type = Operation::Type;
Operation_List extract(const Operation &oper, Message &message)
{
  Operation_List rt;
  if (oper.type == Type::message)
  {
    // TODO: cut 与 替换
  }

  rt += oper;
  return rt;
}

void Operation_List::upgrade(Message &message)
{
  Operation_List rt;
  for (auto &i : list)
    rt += extract(i, message);
  list.clear();
  list = rt.list;
}
