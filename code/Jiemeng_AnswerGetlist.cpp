#include "Jiemeng_Answer.hpp"
#include "Jiemeng_LogIO.hpp"
#include "Jiemeng_Exception.hpp"
using Type = Operation::Type;
Operation_List All_Answer::get_list(const Message &message) const
{
  Operation_List rt;
  for (auto i : answer_list)
  {
    try
    {
      rt = i->get_list(message);
      return rt;
    }
    catch (Not_Serious)
    {
      continue;
    }
  }
  throw Not_Serious();
}
Operation_List Answer_List::get_list(const Message &message) const
{
  Operation_List rt;
  for (auto i : answer_group)
  {
    if (i->check(message))
    {
      rt += i->get_list();
      if (rt.list.back().type == Type::ignore)
        continue;
      else
        return rt;
    }
  }
  dout << "Empty!\n";
  throw Not_Serious();
}

Operation_List Answer::get_list() const
{
  Operation_List rt;
  if (is_leaf())
    return rt + operation;
  if (is_and())
  {
    for (auto &i : sub_ans)
      rt += i->get_list();
    return rt;
  }
  else
    return sub_ans[rand_get()]->get_list();
}
