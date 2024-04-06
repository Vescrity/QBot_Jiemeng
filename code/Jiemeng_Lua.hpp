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
  /// @brief 调用一个 Lua 函数，并将 Message 转为 Lua table 传递
  /// @param func_name 函数名
  /// @param  msg 需要传递的消息
  string call(const string &func_name, Message &msg);
  /// @brief 执行 Lua 代码
  /// @param  代码字符串
  /// @return 返回值
  string exec(const string &);
};

#endif