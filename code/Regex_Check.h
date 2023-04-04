
#ifndef REGEX_CHECK
#define REGEX_CHECK

#include <regex>
#include "json.hpp"
#include <chrono>
#include <future>
#include <functional>
#include "Func_Timeout.h"
using json = nlohmann::json;
using namespace std;
inline bool reg_str_check(const char *msg, const string &rstr)
{
  regex reg(rstr);
  return regex_search(msg, reg);
}
bool reg_or_check(const char *msg, const json &regs);
bool reg_and_check(const char *msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (!reg_or_check(msg, regs[i]))
        return 0;
    }
    return 1;
  }
  else if (regs.is_object())
    return !reg_or_check(msg, regs["not"]);
  else
    return reg_str_check(msg, string(regs));
}
bool reg_or_check(const char *msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (reg_and_check(msg, regs[i]))
        return 1;
    }
    return 0;
  }
  else if (regs.is_object())
    return !reg_or_check(msg, regs["not"]);
  else
    return reg_str_check(msg, string(regs));
}
bool regCheck(const string &msg, const string &regs)
{
  return reg_or_check(msg.c_str(), json::parse(regs));
}
bool reg_check(const char *msg, const json &regs)
{
  try
  { //???
    auto rs = call_with_timeout(std::function<bool(string, string)>(regCheck), 500, string(msg), to_string(regs));
    return rs.get();
  }
  catch (...)
  {
    return 0;
  }
}

#endif