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
void Answer_List::init(const json &js)
{
  clear();
  for (auto &i : js["Answers"])
  {
    Answer_Group *ans = new Answer_Group(i);
    answer_group.push_back(ans);
  }
  sort(answer_group.begin(), answer_group.end(), [](const Answer_Group *a, const Answer_Group *b)
       { return *a < *b; });
}
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
  Answer_List *j = main_answer_read(custom);
  Answer_List *k;
  k = main_answer;
  main_answer = j;
  delete k;
  return;
}
void All_Answer::init(const json &custom)
{
  try
  {
    clear();
    main_answer = main_answer_read(custom);
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
    grps.push_back("private_true"s);
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
  leaf = 0;
  if (js.is_string())
  {
    leaf = 1;
    operation.type = Operation::Type::message;
    operation.str = js;
  }
  else if (js.is_array())
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
      // TODO: 优化写法！
      leaf = 1;
      if (js.count("order"))
      {
        operation.type = Operation::Type::order;
        if (!js["order"].is_string())
          throw invalid_argument("order 被赋了错误的类型。");
        operation.str = js["order"];
        if (operation.str == "ignore")
        {
          operation.type = Operation::Type::ignore;
        }
        else if (operation.str == "clear")
        {
          operation.type = Operation::Type::clear;
        }
      }
      else if (js.count("draw_deck"))
      {
        operation.type = Operation::Type::draw_deck;
        if (!js["draw_deck"].is_string())
          throw invalid_argument("draw_deck 被赋了错误的类型。");
        operation.str = js["draw_deck"];
      }
      else if (js.count("text"))
      {
        operation.type = Operation::Type::message;
        if (!js["text"].is_string())
          throw invalid_argument("text 被赋了错误的类型。");
        operation.str = js["text"];
      }
      else if (js.count("sleep"))
      {
        operation.type = Operation::Type::sleep;
        operation.data = js["sleep"];
        if (!operation.data.is_number())
        {
          // TODO: 情况处理
          throw invalid_argument("Sleep expect NUMBER");
        }
      }
      else if (js.count("ignore"))
      {
        operation.type = Operation::Type::ignore;
      }
      else if (js.count("clear"))
      {
        operation.type = Operation::Type::clear;
      }
      else if (js.count("lua_once"))
      {
        operation.type = Operation::Type::lua_once;
        operation.str = js["lua_once"];
      }
      else if (js.count("lua_call"))
      {
        operation.type = Operation::Type::lua_call;
        operation.str = js["lua_call"];
      }
      else if (js.count("call_state"))
      {
        operation.type = Operation::Type::call_state;
        operation.str = js["call_state"];
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
  else
  {
    for (auto &i : js)
    {
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
