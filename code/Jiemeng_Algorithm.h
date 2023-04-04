#ifndef JIEMENG_ALGORITHM
#define JIEMENG_ALGORITHM
#include <algorithm>
#include <vector>
#include <regex>
#include "json.hpp"
#include "opstring.h"
#include "r_adb.h"
#include "Jiemeng_Basic.h"
#include <sstream>
using string = std::string;
using json = nlohmann::json;
template <typename T>
bool vec_remove_first_value(std::vector<T> &vec, const T &value_to_remove)
{
  auto iter = std::find(vec.begin(), vec.end(), value_to_remove);
  if (iter != vec.end())
  {
    *iter = vec.back();
    vec.pop_back();
    return true;
  }
  return false;
}

std::string string_format_with_json(const string &begin, const string &end, const json &js, const std::string &format)
{

  string result;
  string oper = format;
  string tmp;
  size_t it;
ff:
  it = oper.find(begin);
  if (it == string::npos)
  {
    result += oper;
    return result;
  }
  else
  {
    result += oper.substr(0, it);
    oper = oper.substr(it + begin.length());
    it = oper.find(end);
    if (it == string::npos)
      throw std::runtime_error("invalid format string");
    tmp = oper.substr(0, it);
    json::json_pointer ptr(tmp);
    try
    {
      if (!js.contains(ptr))
        throw std::invalid_argument("invalid json pointer " + tmp);
    }
    catch (std::exception &e)
    {
      std::string msg = "Exception caught: ";
      msg += e.what();
      error_lable("[Format]");
      error_puts(msg.c_str());
      tmp = "[invalid]";
      goto iv;
    }
    // tmp = js[ptr].dump();
    tmp = js[ptr].dump();
    if (js[ptr].is_number())
    {
    }
    else if (js[ptr].is_string())
      tmp = tmp.substr(1, tmp.size() - 2);
    else
    {
      tmp = js[ptr].dump();
      tmp = escape_string(tmp);
      tmp = tmp.substr(1, tmp.size() - 2);
    }
  // tmp = tmp.substr(1, tmp.size() - 2);
  iv:
    result += tmp;
    oper = oper.substr(it + end.length());
    goto ff;
  }
  /*string b, e, r;
  b = change_to_regex(begin);
  e = change_to_regex(end);
  r = b + "(.*?)" + e;
  std::regex regex(r);
  std::smatch match;
  std::string result = format;
  while (std::regex_search(result, match, regex))
  {
    if (match.size() == 2)
    {
      std::string key = match[1].str();
      try
      {
        json::json_pointer ptr(key);
        std::string value = js[ptr].dump();
        result = str_strchg((begin + key + end).c_str(), value.substr(1, value.size() - 2).c_str(), result.c_str());
        // result = std::regex_replace(result, regex, value.substr(1, value.size() - 2));
      }
      catch (const std::exception &e)
      {
        std::string msg = "Exception caught: ";
        msg += e.what();
        error_lable("[string_format]");
        error_puts(msg.c_str());
        break;
      }
    }
  }

  return result;*/
}
#endif