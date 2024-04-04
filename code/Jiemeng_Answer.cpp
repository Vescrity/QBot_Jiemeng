#include "Jiemeng_Answer.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_Algorithm.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
namespace fs = std::filesystem;
json ans_merge(const std::string &folderPath);

void All_Answer::init(const json &custom)
{
  try
  {
    Answer_List *p = new Answer_List;
    std::ostringstream oss;
    oss << ans_merge("./Answer");
    string strdata = oss.str();
    strdata = string_format_with_json("{~", "}", custom, strdata);
    p->init(json::parse(strdata));
    answer_list.push_back(p);
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Answer_init]")
  }
}
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
  for (auto i : answer_group)
  {
    if (i->check(message))
      return i->get_list();
  }
  dout << "Empty!\n";
  throw Not_Serious();
}

bool Answer_Group::check(const Message &message) const
{
  if (type_check(message.place))
    return regex.check(message.text);
  return 0;
}

bool Answer_Group::type_check(const Message_Place &type) const
{
  dout<<level<<" "<<type.level<<"\n";
  if (type.level < level)
    return 0;
  // 1000 级以上的权限无视规则
  if (type.level >= 1000)
    return 1;
  if (both_flag)
    return group_check(type) && user_check(type);
  else
    return group_check(type) || user_check(type);
}

bool Answer_Group::group_check(const Message_Place &type) const
{
  // 群聊被禁用
  if (grps[0] == "0")
    return 0;
  // 200 级以上的权限在允许群聊的词条中无视规则生效
  if (type.level >= 200 && type.is_group())
    return 1;
  // 允许任意生效群聊
  if (grps[0] == "1")
    return 1;
  // -1，排除后续指定群聊
  if (grps[0] == "-1")
    return !(Vec_Find(grps, type.group_id));
  else
    return (Vec_Find(grps, type.group_id));
}
bool Answer_Group::user_check(const Message_Place &type) const
{
  // 200 级以上
  if (type.level >= 200)
    return 1;
  // 允许任意有效用户
  if (user[0] == "1")
    return 1;
  if (user[0] == "0")
    return 0;
  // -1，排除后续指定用户
  if (user[0] == "-1")
    return !(Vec_Find(user, type.user_id));
  else
    return (Vec_Find(user, type.user_id));
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
  js_getval(level, js, "level", 0);
  // 默认要求同时满足条件
  js_getval(both_flag, js, "both", true);

  try
  {
    regex = Json_Regex(js["regex"]);
    priority = js["pri"];
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
    answer.init(js["anss"]);
  else
    throw std::invalid_argument("argument \"anss\" is not given");
}

void Answer::init(const json &js)
{
  and_flag = 0;
  if (js.is_string())
  {
    leaf = 1;
    operation.type = Operation::Type::message;
    operation.str = js;
  }
  if (js.is_array())
  {
    Array_init(js);
  }
  else if (js.is_object())
  {
    if (js.count("anss"))
    {
      if (js.count("is_and"))
      {
        try
        {
          if (!js["is_and"].is_boolean())
            throw invalid_argument("is_and 被提供了错误的值。应当为 true/false。将按照 true 处理。");
          and_flag = js["is_and"];
        }
        catch (invalid_argument &e)
        {
          JM_EXCEPTION("[Answer]");
          and_flag = true;
        }
      }
      else if (js.count("is_or"))
      {
        try
        {
          if (!js["is_or"].is_boolean())
            throw invalid_argument("is_or 被提供了错误的值。应当为 true/false。将按照 true 处理。");
          and_flag = !js["is_or"];
        }
        catch (invalid_argument &e)
        {
          JM_EXCEPTION("[Answer]");
          and_flag = false;
        }
      }
      Array_init(js["anss"]);
    }
    else
    {
      leaf = 1;
      if (js.count("order"))
      {
        operation.type = Operation::Type::order;
        if (!js["order"].is_string())
          throw invalid_argument("order 被赋了错误的类型。");
        operation.str = js["order"];
      }
      else if (js.count("text"))
      {
        operation.type = Operation::Type::message;
        if (!js["text"].is_string())
          throw invalid_argument("text 被赋了错误的类型。");
        operation.str = js["text"];
      }
      else if (js.count("lua_call"))
      {
        operation.type = Operation::Type::lua_call;
        operation.str = js["lua_call"];
      }
      else if (js.count("lua_shell"))
      {
        operation.type = Operation::Type::lua_shell;
        operation.str = js["lua_shell"];
      }
      if (js.count("data"))
      {
        operation.is_json = 1;
        operation.data = js["data"];
      }
    }
  }
}

void Answer::Array_init(const json &js)
{
  int cnt = js.size();
  if (is_or())
  {
    lvs.push_back(0);
    for (auto &i : js)
    {
      if (!i.is_object())
        lvs.push_back(lvs.back() + 1);
      else if (!i.count("weight"))
        lvs.push_back(lvs.back() + 1);
      else
        lvs.push_back(int(i["weight"]) + lvs.back());
      Answer *ans = new Answer;
      ans->init(i);
      sub_ans.push_back(ans);
    }
  }
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

json ans_merge(const std::string &folderPath)
{
  json mergedJson;
  std::vector<json> answerArrays;
  for (const auto &entry : fs::directory_iterator(folderPath))
  {
    if (entry.path().extension() == ".json")
    {
      std::ifstream file(entry.path());
      if (file.is_open())
      {
        json fileJson;
        file >> fileJson;
        if (fileJson.contains("Answers") && fileJson["Answers"].is_array())
        {
          answerArrays.push_back(fileJson["Answers"]);
        }
        file.close();
      }
      else
      {
#ifndef _WIN32
        throw runtime_error("Failed to open file: "s + string(entry.path()));
#else
        throw runtime_error("Failed to open file: "s);
#endif
      }
    }
  }

  mergedJson["Answers"] = json::array();
  for (const auto &answers : answerArrays)
  {
    mergedJson["Answers"].insert(mergedJson["Answers"].end(), answers.begin(), answers.end());
  }
  return mergedJson;
}
