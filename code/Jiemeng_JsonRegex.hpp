#ifndef JIEMENG_JSONREGEX
#define JIEMENG_JSONREGEX
#include <nlohmann/json.hpp>
#include <string>
using string = std::string;
using json = nlohmann::json;
class Json_Regex
{
  json regex;

public:
  bool check(const std::string &msg) const;
  Json_Regex(){}
  Json_Regex(const nlohmann::json &r) : regex(r) {}
};
#endif