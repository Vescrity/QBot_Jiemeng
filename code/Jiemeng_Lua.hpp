#ifndef JIEMENG_LUA
#define JIEMENG_LUA
#include "Jiemeng_Message.hpp"
#include <memory>
#include <mutex>
#include <sol/sol.hpp>
namespace Jiemeng {
class Bot;
class Lua_Shell {
    Bot* const bot;
    std::unique_ptr<sol::state> lua;
    void init();

  public:
    std::mutex mtx;
    sol::protected_function_result get_func(const string &);
    Lua_Shell(Bot* const b) 
        : bot(b),
        lua(std::make_unique<sol::state>()) { init(); };
    void reload();
    /// @brief 调用一个 Lua 函数，并将 Message 转为 Lua table 传递
    /// @param func_name 函数名
    /// @param  msg 需要传递的消息
    string call(const string &func_name, Message msg);
    /// @brief 执行 Lua 代码
    /// @param  代码字符串
    /// @return 返回值
    string exec(const string &);
    string exec(const string &, const Message &msg);
    void load(const string &);
};
} // namespace Jiemeng

#endif
