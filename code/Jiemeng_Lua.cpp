#include "Jiemeng_Lua.hpp"
#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "Jiemeng_DebugIO.hpp"
namespace fs = std::filesystem;
void Lua_Shell::init(Jiemeng *b)
{
  lua.open_libraries(sol::lib::base);
  bot = b;
  string folder_path = "./luarc";
  for (const auto &entry : fs::directory_iterator(folder_path))
  {
    if (entry.path().extension() == ".lua")
    { // 确保是.lua文件
      try
      {
        // 执行Lua脚本
        lua.script_file(entry.path().string());
        std::cout << "Executed: " << entry.path().filename() << std::endl;
      }
      catch (const sol::error &e)
      {
        std::cerr << "Failed to execute " << entry.path().filename() << " due to: " << e.what() << std::endl;
      }
    }
  }
  // lua["group_output"] = group_output;
  lua.set_function(
      "group_output",
      [this](const string group_id, const string user_id, string message)
      {
        CQMessage ms(message);
        return this->bot->message_output(group_id, user_id, ms); });
}
void Lua_Shell::call(const string &func, Message &message)
{
  dout << "call: " << func;
  sol::table msg_table = lua.create_table_with(
      "user_id", message.place.user_id,
      "user_nm", message.place.user_nm,
      "group_id", message.place.group_id,
      "group_nm", message.place.group_nm,
      "is_group", message.is_group(),
      "text", message.text.str());
  lua[func](msg_table);
}