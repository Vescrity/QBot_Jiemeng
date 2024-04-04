#ifndef JIEMENG_LUA
#define JIEMENG_LUA
#include "Jiemeng_Message.hpp"

#include <sol/sol.hpp>

class Jiemeng;
class Lua_Shell
{
  sol::state lua;
  Jiemeng *bot;

public:
  void init(Jiemeng *b);
  void call(const string &, Message &);
  void exec(const string &);
};

#endif