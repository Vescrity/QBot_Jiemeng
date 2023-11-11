#ifndef JIEMENG_ANS
#define JIEMENG_ANS
#include <vector>
#include <string>
#include <algorithm>
#include "r_adb.h"
#include "json.hpp"
#include "Message_Type.h"
#include "Jiemeng_Oper.h"

using Oper = Answer_Oper::Oper;
class Oper_List
{

public:
  vector<Answer_Oper> list;
  Oper_List &operator+=(const Oper_List &b)
  {
    list.insert(list.end(), b.list.begin(), b.list.end());
    return *this;
  }
  Oper_List &operator+=(const Answer_Oper &b)
  {
    list.push_back(b);
    return *this;
  }

  Oper_List operator+(const Oper_List &b) const
  {
    Oper_List result = *this;
    result += b;
    return result;
  }
  Oper_List operator+(const Answer_Oper &b) const
  {
    Oper_List result = *this;
    result += b;
    return result;
  }
};
class Basic_Answer
{
public:
  bool ifroot;
  Answer_Oper val;
  Basic_Answer() { ifroot = 0; }
  virtual Oper_List get_val() const = 0;
  void Str_init(const json &js)
  {
    ifroot = 1;
    val.oper = Oper::output;
    val.str = js;
  }
  void Obj_init(const json &js)
  {
    ifroot = 1;
    val.oper = (!js.count("order")) ? (Oper::output) : (Oper::order);
    val.str = val.oper == Oper::order ? js["order"] : js["text"];
    if (js.count("msgread"))
      val.msgread = js["msgread"];
    if (js.count("data"))
    {
      val.ifjs = 1;
      val.js = js["data"];
    }
  }
  void Basic_init(const json &js)
  {
    if (js.is_string())
      Str_init(js);
    else if (js.is_object())
      Obj_init(js);
  }
  virtual Basic_Answer *init(const json &) = 0;
};
class Answer_And;
class Answer_Or : public Basic_Answer
{
public:
  vector<int> lvs;
  vector<Answer_And *> sub_ans;
  ~Answer_Or();
  int r_opt() const
  {
    int cnt = sub_ans.size();
    int r = Rands(0, lvs[cnt] - 1);
    for (int i = 1; i <= cnt; i++)
    {
      if (lvs[i] > r)
        return i - 1;
    }
    return cnt - 1;
  }

  Oper_List get_val() const;
  void Array_init(const json &js);
  Answer_Or *init(const json &js)
  {
    if (js.is_object())
    {
      if (!js.count("anss"))
      {
        Obj_init(js);
        return this;
      }
      Array_init(js["anss"]);
    }
    else if (js.is_array())
      Array_init(js);
    else
      Basic_init(js);
    return this;
  }
};
class Answer_And : public Basic_Answer
{
public:
  vector<Answer_Or *> sub_ans;
  ~Answer_And()
  {
    for (auto i : sub_ans)
      delete i;
    sub_ans.clear();
  }
  Oper_List get_val() const
  {
    Oper_List rt;
    if (ifroot)
      return rt + val;
    for (auto &i : sub_ans)
      rt += i->get_val();
    return rt;
  }
  void Array_init(const json &js)
  {
    for (auto &i : js)
    {
      Answer_Or *ans = new Answer_Or;
      sub_ans.push_back(ans->init(i));
    }
  }
  Answer_And *init(const json &js)
  {
    if (js.is_object())
    {
      if (!js.count("anss"))
      {
        Obj_init(js);
        return this;
      }
      Array_init(js["anss"]);
    }
    else if (js.is_array())
      Array_init(js);
    else
      Basic_init(js);
    return this;
  }
};
/// @brief 应答组类
class Answer_Group
{

public:
  vector<string> grps;
  vector<string> user;
  Answer_Or anss;
  json jreg;
  int level;
  int pri;
  bool operator<(const Answer_Group &b) const { return pri > b.pri; }
  bool type_check(const Basic_Message_Type type) const;
  bool check(const Basic_Message_Type type) const;
  void init(const json &js);
};

class Answers
{
public:
  vector<Answer_Group *> ans_group;
  void init(const json &js)
  {
    for (auto &i : js["Answers"])
    {
      Answer_Group *ans = new Answer_Group;
      ans->init(i);
      ans_group.push_back(ans);
    }
    sort(ans_group.begin(), ans_group.end(), [](const Answer_Group *a, const Answer_Group *b)
         { return *a < *b; });
  }
  void clear()
  {
    for (auto i : ans_group)
      delete i;
    ans_group.clear();
  }
  ~Answers() { clear(); }

} answers;
Answer_Or::~Answer_Or()
{
  for (auto i : sub_ans)
    delete i;
  sub_ans.clear();
}
void Answer_Group::init(const json &js)
{
  if (js.count("grps"))
  {
    if (js["grps"].is_array())
      grps = js["grps"].get<vector<string>>();
    else
      throw std::invalid_argument("argument \"grps\" should be an array");
  }
  else
    grps.push_back("1"s);
  if (js.count("user"))
  {
    if (js["user"].is_array())
      user = js["user"].get<vector<string>>();
    else
      throw std::invalid_argument("argument \"user\" should be an array");
  }
  else
    user.push_back("1"s);
  if (js.count("level"))
    level = js["level"];
  else
    level = 0;
  try
  {
    jreg = js["regex"];
    pri = js["pri"];
  }
  catch (const std::out_of_range &e)
  {
    throw std::invalid_argument("argument \"regex\" or \"pri\" is missing: " + std::string(e.what()));
  }
  catch (const std::domain_error &e)
  {
    throw std::invalid_argument("argument \"regex\" or \"pri\" is invalid: " + std::string(e.what()));
  }
  if (js.count("anss"))
    anss.init(js["anss"]);
  else
    throw std::invalid_argument("argument \"anss\" is not given");
}
#ifdef JIEMENG_CONFIG

bool Answer_Group::type_check(const Basic_Message_Type type) const
{
  if (type.admin_level < level)
    return 0;
  if (type.admin_level >= 1000)
    return 1;
  if (type.admin_level >= 200 && type.ifgroup && grps[0] != "0")
    return 1;
  if (grps[0] == "0" && type.ifgroup)
    return 0;
  if ((user[0] == "0" || user[0] == "-0") && (!type.ifgroup))
    return 0;

  if (user[0] == "-1" || (type.ifgroup && user[0] == "-0"))
  {
    if (Vec_Find(user, type.user_id))
      return 0;
  }
  else if (user[0] != "1" && user[0] != "0")
  {
    if (!Vec_Find(user, type.user_id))
      return 0;
  }
  if (!type.ifgroup || grps[0] == "1")
    return 1;
  if (grps[0] == "-1")
  {
    if (Vec_Find(grps, type.group_id))
      return 0;
    else
      return 1;
  }
  return Vec_Find(grps, type.group_id);
}
#include "Regex_Check.h"
bool Answer_Group::check(const Basic_Message_Type type) const
{
  if (!type_check(type))
  {
    return 0;
  }

  return reg_check(type.message.c_str(), jreg);
}
#endif
Oper_List Answer_Or::get_val() const
{
  Oper_List rt;
  return ifroot ? rt + val : sub_ans[r_opt()]->get_val();
}
void Answer_Or::Array_init(const json &js)
{
  int cnt = js.size();
  lvs.push_back(0);
  for (auto &i : js)
  {
    if (!i.is_object())
      lvs.push_back(lvs.back() + 1);
    else if (!i.count("weight"))
      lvs.push_back(lvs.back() + 1);
    else
      lvs.push_back(int(i["weight"]) + lvs.back());
    Answer_And *ans = new Answer_And;
    sub_ans.push_back(ans->init(i));
  }
}

#endif