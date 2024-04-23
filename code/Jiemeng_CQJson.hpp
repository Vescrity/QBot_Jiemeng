#ifndef JIEMENG_CQJSON
#define JIEMENG_CQJSON

#include <string>
#include <nlohmann/json.hpp>
#include "Jiemeng_DebugIO.hpp"

using json = nlohmann::json;
using string = std::string;

class CQMessage
{
  mutable string cq;
  mutable json js;
  mutable bool json_ready, string_ready;
  void generate_json() const;
  void generate_string() const;

public:
  CQMessage() { json_ready = string_ready = 0; }
  CQMessage(const string &s)
  {
    CQMessage();
    cq = s;
    string_ready = 1;
    json_ready = 0;
  }
  CQMessage(const json &s)
  {
    CQMessage();
    js = s;
    json_ready = 1;
    string_ready = 0;
  }
  json &get_json() const
  {
    if (!json_ready)
      generate_json();
    return js;
  }
  string &get_string() const
  {
    if (!string_ready)
      generate_string();
    return cq;
  }
  string &&str()
  {
    get_string();
    json_ready = 0;
    return std::move(cq);
  }
  string const_str() const { return get_string(); }
  string true_str() const;
  void change(const json &j)
  {
    js = j;
    json_ready = 1;
    string_ready = 0;
  }
  void change(const string &s)
  {
    cq = s;
    json_ready = 0;
    string_ready = 1;
  }
  void change(const char *s)
  {
    change(string(s));
  }
  CQMessage &operator=(const string &s)
  {
    change(s);
    return *this;
  }
  // operator std::string() { return const_str(); }
};
#endif
