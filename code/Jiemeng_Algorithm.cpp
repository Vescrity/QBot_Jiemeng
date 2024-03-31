#include "Jiemeng_Algorithm.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void Get_Para(const std::string &str, int n, std::vector<std::string> *result, const std::string &cutstr)
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

        if (js[ptr].is_number())
        {
          str = js[ptr].dump();
        }
        else if (js[ptr].is_string())
        {
          str = js[ptr];
          // str = str.substr(1, str.size() - 2);
        }
        else if (js[ptr].is_boolean())
          str = js[ptr] ? "1" : "0";
        else
        {
          str = js[ptr].dump();
          // str = escape_string(str);
          str = str.substr(1, str.size() - 2);
        }
        return str;
      });
}