#include "Jiemeng_Lua.hpp"

void Lua_Engine::init()
{
  L = luaL_newstate(); // 创建一个新的Lua环境
  luaL_openlibs(L);    // 打开Lua标准库
  // 注册函数
  redirect_lua_print(L, output);
}

bool Lua_Engine::exec_script(const std::string &filename)
{
  if (luaL_dofile(L, filename.c_str()) != LUA_OK)
  {
    const char *error = lua_tostring(L, -1);
    std::cerr << "Lua Error: " << error << std::endl;
    return false;
  }
  return true;
}
bool Lua_Engine::exec_code(const std::string &code)
{
  if (luaL_dostring(L, code.c_str()) != LUA_OK)
  {
    const char *error = lua_tostring(L, -1);
    std::cerr << "Lua Error: " << error << std::endl;
    return false;
  }
  return true;
}

int lua_custom_print(lua_State *L)
{
  std::stringstream &out = *static_cast<std::stringstream *>(lua_touserdata(L, lua_upvalueindex(1)));
  int nargs = lua_gettop(L);
  for (int i = 1; i <= nargs; i++)
  {
    if (lua_isstring(L, i))
    {
      out << lua_tostring(L, i);
    }
    else
    {
      // 如果不是字符串，调用默认的tostring方法来转换
      lua_pushvalue(L, i); // 把参数压栈
      lua_call(L, 1, 1);
      out << lua_tostring(L, -1); // 获取转换结果
      lua_pop(L, 1);              // 弹出结果
    }
    if (i < nargs)
      out << "\t";
  }
  out << std::endl; // 添加一个换行符
  return 0;
}
// 用来重定向Lua的print到我们的自定义函数
void redirect_lua_print(lua_State *L, std::stringstream &out)
{
  lua_pushlightuserdata(L, &out);
  lua_pushcclosure(L, lua_custom_print, 1);
  lua_setglobal(L, "print");
}