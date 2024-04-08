#include "Jiemeng_JsonRegex.hpp"
#include "Jiemeng_Exception.hpp"
#include <boost/regex.hpp>

bool reg_check(const string &msg, const json &regs);
inline bool reg_str_check(const string &msg, const std::string &rstr)
{
  boost::regex reg(rstr);
  return boost::regex_search(msg, reg);
}
bool reg_and_check(const string &msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (!reg_check(msg, regs[i]))
        return 0;
    }
    return 1;
  }
  else
    return reg_check(msg, regs);
}
bool reg_or_check(const string &msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (reg_check(msg, regs[i]))
        return 1;
    }
    return 0;
  }
  else
    return reg_check(msg, regs);
}

bool reg_check(const string &msg, const json &regs)
{
  try
  {
    if (regs.is_string())
      return reg_str_check(msg, regs);
    if (regs.is_array())
      return reg_or_check(msg, regs);
    if (regs.is_object())
    {
      if (regs.contains("and"))
        return reg_and_check(msg, regs["and"]);
      if (regs.contains("or"))
        return reg_or_check(msg, regs["or"]);
      if (regs.contains("not"))
        return !reg_check(msg, regs["not"]);
    }
    throw invalid_argument("无效的 regex 值。");
  }
  catch (const std::exception &e)
  {
    JM_EXCEPTION("[Regex]")
    error_print("The json reg is: ");
    error_print(regs.dump().c_str());
    return 0;
  }
}

bool Json_Regex::check(const string &msg) const { return reg_check(msg, regex); }