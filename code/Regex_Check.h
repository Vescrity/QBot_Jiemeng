
#ifndef REGEX_CHECK
#define REGEX_CHECK

#include <nlohmann/json.hpp>
#include "Func_Timeout.h"
#include "Jiemeng_Exception.h"
#include <boost/regex.hpp>
using json = nlohmann::json;
using namespace std;
inline bool reg_str_check(const char *msg, const std::string &rstr)
{
  boost::regex reg(rstr);
  return boost::regex_search(msg, reg);
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

bool reg_check(const char *msg, const json &regs)
{
  try
  {
    return reg_or_check(msg, regs);
  }
  catch (const std::exception &e)
  {
    JM_EXCEPTION("[Regex]")
    error_print("The json reg is: ");
    cerr << regs << endl;
    return 0;
  }
}

#endif