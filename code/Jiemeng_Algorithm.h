#ifndef JIEMENG_ALGORITHM
#define JIEMENG_ALGORITHM

#include <algorithm>
#include <vector>
#include "opstring.h"
#include "r_adb.h"
#include "Jiemeng_Basic.h"
#include "Jiemeng_Exception.h"
#include <sstream>
using string = std::string;


#define Vec_Find(a, b) (find((a).begin(), (a).end(), b) != (a).end())
/// @brief Removes the first element equal to a given value from the given vector.
/// @tparam T Any type that has overloaded the equality (==) operator
/// @param vec The given vector
/// @param value_to_remove The value need to remove
/// @return true for remove success, false for no element equal to the given value.
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
/// @brief Splits a given string into n parts using a specified delimiter and stores the results in a vector.
/// @param str The string to split.
/// @param n The number of parts to split the string into.
/// @param result Pointer to a vector to store the results in. The vector is cleared before the results are stored.
/// @param cutstr The delimiter to use for splitting the string. Defaults to "#".
/// @throws std::runtime_error if the string can't be split into n parts.
void Get_Para(const std::string &str, int n, std::vector<std::string> *result, const std::string &cutstr = "#")
{
  result->clear();
  size_t pos = 0;
  for (int i = 0; i < n - 1; ++i)
  {
    size_t nextPos = str.find(cutstr, pos);
    if (nextPos == std::string::npos)
    {
      throw std::runtime_error("The string can't cut to n parts");
    }
    result->push_back(str.substr(pos, nextPos - pos));
    pos = nextPos + cutstr.length();
  }
  if (pos >= str.size())
  {
    throw std::runtime_error("The string can't cut to n parts");
  }
  result->push_back(str.substr(pos));
}

/// @brief replaces text enclosed between `begin` and `end` delimiters with the result of a user-defined rule.
/// @param begin The beginning delimiter for the text to be replaced.
/// @param end The ending delimiter for the text to be replaced.
/// @param input the whole string need to replace
/// @param rule rule to replace
/// @return string after replace
std::string Basic_String_Replace(const string &begin, const string &end, const string &input, std::function<std::string(const string &)> rule)
{
  string result;
  std::string_view oper(input);
  size_t it, it2;
  bool flg = 0;
  if (begin.empty() && end.empty())
    throw std::invalid_argument("Begin and end can't be empty at the same time");
ff:
  it = oper.find(begin);
  if (it == string::npos)
  {
    result += oper;
    if (!flg)
      return result;
    else
    {
      oper = string_view(result);
      goto ff;
    }
  }
  else
  {
    result += oper.substr(0, it);
    oper.remove_prefix(it + begin.length());
    it = oper.find(end);
    it2 = oper.find(begin);
    if (it > it2)
    {
      result += begin;
      flg = 1;
      goto ff;
    }
    if (it == string::npos)
      throw std::runtime_error("invalid input string");
    result += rule(string(oper.substr(0, it)));
    oper.remove_prefix(it + end.length());
    goto ff;
  }
}

#ifdef INCLUDE_NLOHMANN_JSON_HPP_

using json = nlohmann::json;
/// @brief According to the json_pointer between `begin` and `end`, replace the given string.
/// @param begin The beginning delimiter for the text to be replaced.
/// @param end The ending delimiter for the text to be replaced.
/// @param js The given json
/// @param format The string need to replace
/// @return String after change
std::string string_format_with_json(const string &begin, const string &end, const json &js, const std::string &format)
{
  return Basic_String_Replace(
      begin, end, format,
      [&js](const string &tmp)
      {
        json::json_pointer ptr(tmp);
        string str;
        try
        {
          if (!js.contains(ptr))
            throw std::invalid_argument("invalid json pointer " + tmp);
        }
        catch (std::exception &e)
        {
          JM_EXCEPTION("[Format]")
          str = "[invalid]";
          return str;
        }
        str = js[ptr].dump();
        if (js[ptr].is_number())
        {
        }
        else if (js[ptr].is_string())
          str = str.substr(1, str.size() - 2);
        else if (js[ptr].is_boolean())
          str = js[ptr] ? "1" : "0";
        else
        {
          str = escape_string(str);
          str = str.substr(1, str.size() - 2);
        }
        return str;
      });
}
#endif

#endif