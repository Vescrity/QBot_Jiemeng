#ifndef JIEMENG_DEBUGIO
#define JIEMENG_DEBUGIO

#include "Jiemeng_IO.hpp"
#include <nlohmann/json.hpp>
#include <string>
using namespace std;
using json = nlohmann::json;

class DoutStream
{
public:
  template <typename T>
  DoutStream &operator<<(const T &value)
  {
    dprint(std::to_string(value).c_str());
    return *this;
  }
  DoutStream &operator<<(const char *value)
  {
    dprint(value);
    return *this;
  }
  DoutStream &operator<<(const string &value)
  {
    dprint(value.c_str());
    return *this;
  }
  DoutStream &operator<<(const nlohmann::json &json)
  {
    dprint(json.dump().c_str());
    return *this;
  }

private:
  void dprint(const char *message) { debug_print(message); }
};
extern DoutStream dout;

#endif