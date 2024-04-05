#ifndef JIEMENG_LUA
#define JIEMENG_LUA
#include "Jiemeng_Message.hpp"

#include <sol/sol.hpp>

class Jiemeng;
class Lua_Shell
{
  sol::state lua;
  Jiemeng *bot;
  void init(Jiemeng *b);

public:
  Lua_Shell(Jiemeng *b) { init(b); };
  void call(const string &, Message &);
  string exec(const string &);
};

#endif