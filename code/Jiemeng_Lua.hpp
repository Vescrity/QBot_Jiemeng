#ifndef JIEMENG_LUA
#define JIEMENG_LUA
#include <string>
#include <sstream>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
void redirect_lua_print(lua_State *L, std::stringstream &out);

// typedef int (*LuaCFunction)(lua_State *);

class Lua_Engine
{
public:
  void init();
  Lua_Engine() { init(); }
  void clear()
  {
    if (L)
      lua_close(L);
  }
  ~Lua_Engine() { clear(); }
  void reset()
  {
    clear();
    Lua_Engine();
  }
  bool exec_script(const std::string &filename);
  // 执行Lua代码块
  bool exec_code(const std::string &code);
  /*void reg_func(const std::string &name, LuaCFunction function)
  {
    lua_pushcfunction(L, function); // 把C++函数推送到Lua栈上
    lua_setglobal(L, name.c_str()); // 设置为Lua全局函数
  }*/
protected:
  template <typename T, int (T::*func)(lua_State *)>
  static int dispatch(lua_State *L)
  {
    T *ptr = *static_cast<T **>(lua_getextraspace(L));
    return (ptr->*func)(L);
  }
  template <typename T, int (T::*func)(lua_State *)>
  void reg_func(const char *name)
  {
    lua_pushcfunction(L, (&dispatch<T, func>));
    lua_setglobal(L, name);
  }
  lua_State *L; // 指向Lua环境的指针
private:
  std::stringstream output;
};

class Lua_Shell : public Lua_Engine
{
public:
  Lua_Shell()
  {
    *static_cast<Lua_Shell **>(lua_getextraspace(L)) = this;
    reg_func<Lua_Shell, &Lua_Shell::setA>("setA");
  }
  /// @brief 示例函数
  int setA(lua_State *L)
  {
    a = luaL_checknumber(L, 1);
    return 0;
  }

private:
  /// 测试变量
  int a;
};

#endif