#ifndef JIEMENG_ALGORITHM
#define JIEMENG_ALGORITHM

#include <algorithm>
#include <vector>
#include "opstring.h"
#include "Jiemeng_Random.hpp"
#include "Jiemeng_Basic.h"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_JsonTemplate.hpp"
#include <sstream>
#include <memory>
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
/// @brief 数组中是否含有 a
/// @param a
/// @param b
/// @return
template <typename T, size_t N>
bool array_search(const T &a, const T (&b)[N])
{
  for (size_t i = 0; i < N; ++i)
  {
    if (a == b[i])
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
void Get_Para(const std::string &str, int n, std::vector<std::string> *result, const std::string &cutstr = "#");

/// @brief replaces text enclosed between `begin` and `end` delimiters with the result of a user-defined rule.
/// @param begin The beginning delimiter for the text to be replaced.
/// @param end The ending delimiter for the text to be replaced.
/// @param input the whole string need to replace
/// @param rule rule to replace
/// @return string after replace
std::string Basic_String_Replace(const string &begin, const string &end, const string &input, std::function<std::string(const string &)> rule);

using json = nlohmann::json;
/// @brief According to the json_pointer between `begin` and `end`, replace the given string.
/// @param begin The beginning delimiter for the text to be replaced.
/// @param end The ending delimiter for the text to be replaced.
/// @param js The given json
/// @param format The string need to replace
/// @return String after change
std::string string_format_with_json(const string &begin, const string &end, const json &js, const std::string &format);
/// @brief 分割字符
/// @param str
/// @param cut
/// @return
std::unique_ptr<std::vector<std::string>> string_cut(const std::string &str, const std::string &cut, const int &n = -1);
#endif