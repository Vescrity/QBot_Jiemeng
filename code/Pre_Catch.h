#ifndef PRE_CATCH
#define PRE_CATCH
#include <string>
#include <nlohmann/json.hpp>
#include "Message_Type.h"
#include "Jiemeng_Ans.h"
#include "Regex_Check.h"
class Pre_Catch
{
public:
  json jreg;
  Answer_Or anss;
  Basic_Message_Type type;
  // 最低触发等级
  int level;
  bool survive;
  enum class Type_Check
  {
    same,
    group,
    user,
    // specific,
    all
  } t_check;
  bool Type_Check(const Basic_Message_Type &tp) const
  {
    if (tp.admin_level < level)
      return 0;
    if (t_check == Type_Check::same)
      return type.same(tp);
    if (t_check == Type_Check::group)
      return type.group_id == tp.group_id;
    if (t_check == Type_Check::user)
      return type.group_id == tp.user_id;
    if (t_check == Type_Check::all)
      return 1;
    else
      throw std::invalid_argument("undifined type_check type");
  }
  bool Check(const Basic_Message_Type &tp) const
  {
    if (Type_Check(tp))
      return reg_check(tp.message.c_str(), jreg);
    return 0;
  }
  void init(const json &js)
  {
    if (js.count("regex"))
      jreg = js["regex"];
    else
      throw std::invalid_argument("The value of Regex at Pre_catch is not provided");
    if (!js.count("anss"))
      throw std::invalid_argument("The value of anss at Pre_catch is not provided");
    anss.init(js["anss"]);
    if (!js.count("level"))
      level = 0;
    else
      level = js["level"];
    if (!js.count("survive"))
      survive = 0;
    else
      survive = js["survive"];
    if (!js.count("type"))
      t_check = Type_Check::same;
    else
    {
      string type = js["type"];
      if (type == "same")
        t_check = Type_Check::same;
      else if (type == "group")
        t_check = Type_Check::group;
      else if (type == "user")
        t_check = Type_Check::user;
      else if (type == "all")
        t_check = Type_Check::all;
      else
        throw std::invalid_argument("Undefined Check_Type at Pre_catch");
    }
  }
};
#include <iostream>
class Pre_Catch_List
{
private:
  vector<Pre_Catch *> pre_catch;

public:
  ~Pre_Catch_List()
  {
    for (auto i : pre_catch)
      delete i;
    pre_catch.clear();
  }
  bool Operate(const Message_Type &type);
  void push(Pre_Catch *pr) { pre_catch.push_back(pr); }
  void push(const json &js)
  {
    Pre_Catch *pr = new Pre_Catch;
    pr->init(js);
    pre_catch.push_back(pr);
  }
} pre_catch_list;
void Pre_Catch_Init()
{
  // .info
  {
    string version_info = string(JIEMENG_PLATFORM) + " " + JIEMENG_VERSION;
    version_info = version_info +
                   "\nCompile at " + UPDATE_TIME +
                   "\nDeveloped by Yukitoha\n"
                   "More information at https://github.com/Vescrity/QBot_Jiemeng\n"
                   "Do not use me too hard♡";
    json _INFO = {
        {"regex", "^\\.info$"},
        {"anss", json::array({version_info})},
        {"survive", true},
        {"type", "all"}};
    pre_catch_list.push(_INFO);
  }
  // order
  {
    json _ORDER = R"(
      {
        "regex": "^/#",
        "anss": [
          {
            "order":"order",
            "msgread":true
          }
        ],
        "level":100,
        "survive":true,
        "type":"all"
      }
    )"_json;
    pre_catch_list.push(_ORDER);
  }
}
#endif