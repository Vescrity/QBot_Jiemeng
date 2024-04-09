#include "Jiemeng_Answer.hpp"
#include "Jiemeng_LogIO.hpp"
#include "Jiemeng_Exception.hpp"
using Type = Operation::Type;
Operation_List All_Answer::get_list(const Message &message) const
{
  Operation_List rt;
  debug_lable("[Get_List]");
  dout << "开始检索匹配\n";
  for (auto &i : answer_list)
  {
    try
    {
      rt = i->get_list(message);
      debug_lable("[Get_List]");
      dout << "检索完成！已成功生成 Operation_LIst\n";
      return rt;
    }
    catch (Not_Serious)
    {
      continue;
    }
  }
  debug_lable("[Get_List]");
  dout << "检索完成！没有发现匹配项。\n";
  throw Not_Serious();
}
Operation_List Answer_List::get_list(const Message &message) const
{
  Operation_List rt;
  for (auto &i : answer_group)
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

int Answer::rand_get() const
{
  int cnt = sub_ans.size();
  int r = Rands(0, lvs[cnt] - 1);
  // 查找，或许可以考虑二分
  for (int i = 1; i <= cnt; i++)
  {
    if (lvs[i] > r)
      return i - 1;
  }
  return cnt - 1;
}