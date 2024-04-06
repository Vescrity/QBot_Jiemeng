#include "Jiemeng_Lua.hpp"
#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "Jiemeng_LogIO.hpp"
#include "Jiemeng_Version.hpp"
#include "Jiemeng_Deck.hpp"
namespace fs = std::filesystem;
void Lua_Shell::init(Jiemeng *b)
{
  lua.open_libraries();
  bot = b;
  // string folder_path = "./luarc";
  auto lua_load = [this](const string &path)
  {
    for (const auto &entry : fs::directory_iterator(path))
    {
      if (entry.path().extension() == ".lua")
      {
        try
        {
          // 执行Lua脚本
          lua.script_file(entry.path().string());
          dout << "Executed: " << string(entry.path().filename()) << "\n";
        }
        catch (const sol::error &e)
        {
          error_puts(string("Failed to execute ") + string(entry.path().filename()));
          JM_EXCEPTION("[Lua_Shell]")
        }
      }
    }
  };
  lua_load("./luarc");
  lua_load("./user_luarc");
  lua["_version"] = JIEMENG_VERSION;
  lua["_platform"] = JIEMENG_PLATFORM;
  lua["_compile_time"] = UPDATE_TIME;
  lua.set_function(
      "group_output",
      [this](const string group_id, string message)
      {
        CQMessage ms(message);
        return this->bot->group_output(group_id, ms); });
  lua.set_function(
      "private_output",
      [this](const string user_id, string message)
      {
        CQMessage ms(message);
        return this->bot->private_output(user_id, ms); });
  lua.set_function(
      "deck_draw",
      [this](const string key)
      { return this->bot->deck->draw(key); });
}
void Lua_Shell::call(const string &func, Message &message)
{
  dout << "call: " << func << "\n";
  try
  {
    sol::table msg_table = lua.create_table_with(
        "user_id", message.place.user_id,
        "user_nm", message.place.user_nm,
        "group_id", message.place.group_id,
        "group_nm", message.place.group_nm,
        "is_group", message.is_group(),
        "true_text", message.text.true_str(),
        "text", message.text.const_str());
    auto result = lua[func](msg_table);
    if (!result.valid())
      throw sol::error(result);
  }
  catch (const sol::error &e)
  {
    JM_EXCEPTION("[Lua_Call]")
  }
}
string Lua_Shell::exec(const string &code)
{
  string str;
  try
  {
    str = lua.script(code);
  }
  catch (const sol::error &e)
  {
    JM_EXCEPTION("[Lua_Call]")
  }
  return str;
}