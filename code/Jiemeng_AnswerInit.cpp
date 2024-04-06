#include "Jiemeng_Answer.hpp"
#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_Algorithm.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
namespace fs = std::filesystem;
using Type = Operation::Type;
json ans_merge(const std::string &folderPath);
Answer_List *main_answer_read(const json &custom)
{
  Answer_List *p = new Answer_List;
  std::ostringstream oss;
  oss << ans_merge("./Answer");
  string strdata = oss.str();
  strdata = string_format_with_json("{~", "}", custom, strdata);
  p->init(json::parse(strdata));
  return p;
}
void All_Answer::main_answer_reload(const json &custom)
{
  for (auto i : answer_list)
  {
    if (i->priority == 0)
    {
      delete i;
      i = main_answer_read(custom);
      return;
    }
  }
}
void All_Answer::init(const json &custom)
{
  try
  {
    clear();
    Answer_List *p = main_answer_read(custom);
    answer_list.push_back(p);
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Answer_init]")
  }
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
    if (js.count("and"))
    {
      and_flag = true;
      Array_init(js["and"]);
    }
    else if (js.count("or"))
    {
      and_flag = false;
      Array_init(js["or"]);
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
      else if (js.count("ignore"))
      {
        operation.type = Operation::Type::ignore;
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
  // ignore 替换
  if (operation.type == Type::message)
  {
    if (operation.str == "[ignore]")
      operation.type = Type::ignore;
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
