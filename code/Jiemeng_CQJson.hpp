#ifndef JIEMENG_CQJSON
#define JIEMENG_CQJSON

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using string = std::string;

class CQMessage
{
  string cq;
  json js;
  bool json_ready, string_ready;
  void generate_json();
  void generate_string();

public:
  CQMessage() {  json_ready = string_ready = 0; }
  CQMessage(const string &s)
  {
    CQMessage();
    cq = s;
    string_ready = 1;
  }
  CQMessage(const json &s)
  {
    CQMessage();
    js = s;
    json_ready = 1;
  }
  json &get_json()
  {
    if (!json_ready)
      generate_json();
    return js;
  }
  string &get_string()
  {
    if (!string_ready)
      generate_string();
    return cq;
  }
  string &str()
  {
    json_ready = 0;
    return get_string();
  }
  string const_str() { return get_string(); }
  string true_str();
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
    cq = s;
    json_ready = 0;
    string_ready = 1;
  }
  CQMessage &operator=(const string &s)
  {
    change(s);
    return *this;
  }
  operator std::string() const { return cq; }
};
#endif
