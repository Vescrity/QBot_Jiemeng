#include "Jiemeng_Lua.hpp"
#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "Jiemeng_LogIO.hpp"
#include "Jiemeng_Version.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_MessageReplace.hpp"
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

  lua.new_usertype<Message_Place>(
      "Message_Place",
      "new", sol::constructors<Message_Place()>(),
      "group_id", &Message_Place::group_id,
      "group_nm", &Message_Place::group_nm,
      "user_id", &Message_Place::user_id,
      "user_nm", &Message_Place::user_nm,
      "level", &Message_Place::level,
      "set_group", &Message_Place::set_group,
      "set_private", &Message_Place::set_private,
      "is_group", &Message_Place::is_group,
      "is_private", &Message_Place::is_private);
  lua.new_usertype<Message>(
      "Message",
      "new", sol::constructors<Message()>(),
      "str", &Message::str,
      "change", &Message::change,
      "true_str", &Message::true_str,
      "show", &Message::show,
      "is_group", &Message::is_group,
      "is_private", &Message::is_private,
      "place", &Message::place);
  lua.new_usertype<Operation>(
      "Operation",
      "new", sol::constructors<Operation()>(),
      "str", &Operation::str,
      "set_type", &Operation::set_type);
  lua.new_usertype<Operation_List>(
      "Operation_List",
      "new", sol::constructors<Operation_List()>(),
      "push_back", &Operation_List::push_back);
  sol::table botlib = lua.create_table();
  botlib.set_function(
      "group_output",
      [this](const string group_id, string message)
      {
        CQMessage ms(message);
        return this->bot->group_output(group_id, ms); });
  botlib.set_function(
      "private_output",
      [this](const string user_id, string message)
      {
        CQMessage ms(message);
        return this->bot->private_output(user_id, ms); });
  botlib.set_function(
      "_deck_draw",
      [this](const string key)
      { return this->bot->deck->draw(key); });
  botlib.set_function(
      "message_replace",
      [this](const string str, Message_Place place)
      {
        string s = str;
        message_replace(s, place);
        return s;
      });
  botlib.set_function(
      "process_message",
      [this](Message message)
      { return this->bot->process_message(message); });
  botlib.set_function(
      "process_operation",
      [this](Message message, Operation_List list)
      { return this->bot->process_operation(message, list); });
  lua["bot"] = botlib;
  lua_load("./luarc");
  lua_load("./user_luarc");
  lua["bot"]["_version"] = JIEMENG_VERSION;
  lua["bot"]["_platform"] = JIEMENG_PLATFORM;
  lua["bot"]["_compile_time"] = UPDATE_TIME;
  lua["bot"]["spliter"] = bot->config.spliter;
}
string Lua_Shell::call(const string &func, Message &message)
{
  debug_lable("[Lua_Call]");
  dout << "call: " << func << "\n";
  try
  {
    /*sol::table msg_table = lua.create_table_with(
        "user_id", message.place.user_id,
        "user_nm", message.place.user_nm,
        "group_id", message.place.group_id,
        "group_nm", message.place.group_nm,
        "is_group", message.is_group(),
        "true_text", message.text.true_str(),
        "text", message.text.const_str());*/
    auto fun = lua.script("return "s + func);
    sol::function fu = fun;
    auto result = fu(message);
    // auto result = lua[func](message);
    if (!result.valid())
      throw sol::error(result);
    return lua["tostring"](result);
    /*if (result.get_type() == sol::type::string)
    {
      return result;
    }
    if (result.get_type() == sol::type::number)
    {
      return to_string(double(result));
    }*/
    return "";
  }
  catch (const sol::error &e)
  {
    JM_EXCEPTION("[Lua_Call]")
    return "";
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
