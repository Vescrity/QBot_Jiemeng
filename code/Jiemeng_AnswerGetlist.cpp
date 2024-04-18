#include "Jiemeng_Answer.hpp"
#include "Jiemeng_LogIO.hpp"
#include "Jiemeng_Exception.hpp"
using Type = Operation::Type;
Operation_List All_Answer::get_list(const Message &message) const
{
  Operation_List rt;
  auto b=message;
  debug_lable("[Get_List]");
  dout << "开始检索匹配\n";
  auto c = [&](const vector<Answer_List *> &x)
  {
    for (auto &i : x)
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
    throw Not_Serious();
  };
  try  {return c(pre_answer_list);  }
  catch (Not_Serious){}
  try  {return main_answer->get_list(message);}
  catch(Not_Serious) {}
  try{return c(suf_answer_list);}
  catch (Not_Serious){}
  debug_lable("[Get_List]");
  debug_puts( "检索完成！没有发现匹配项。");
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
  // 查找，数据量较小决定不二分
  for (int i = 1; i <= cnt; i++)
  {
    if (lvs[i] > r)
      return i - 1;
  }
  return cnt - 1;
}