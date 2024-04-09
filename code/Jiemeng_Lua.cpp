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
#include "Jiemeng_Request.hpp"
namespace fs = std::filesystem;
nlohmann::json lua_table_to_json(sol::object lua_value);
sol::object json_to_lua_table(const nlohmann::json &j, sol::state &lua);

void Lua_Shell::load(const string &path)
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
}

void Lua_Shell::init(Jiemeng *b)
{
  lua.open_libraries();
  bot = b;

  lua.new_usertype<json>(
      "json",
      "new", sol::constructors<json()>(),
      "dump",
      sol::overload(
          [](json &js)
          { return js.dump(); },
          [](json &js, int n)
          { return js.dump(n); }));
  lua.new_usertype<Request>(
      "Request",
      "new", sol::constructors<Request()>(),
      "set_url", &Request::set_url,
      "set_api", &Request::set_api,
      "set_data", &Request::set_data,
      "set_msgs", &Request::set_msgs,
      "Post", &Request::Post,
      "js_post", &Request::js_post,
      "js_get", &Request::js_get,
      "Get", &Request::Get);
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
  sol::table jsonlib = lua.create_table();
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
      "_draw_deck",
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
  botlib.set_function(
      "answer_reload",
      [this]()
      { return this->bot->answer_reload(); });
  botlib.set_function(
      "deck_reload",
      [this]()
      { return this->bot->deck_reload(); });
  jsonlib.set_function(
      "table2json",
      lua_table_to_json);
  jsonlib.set_function(
      "json2table",
      [&](json &js)
      { return json_to_lua_table(js, lua); });
  lua["bot"] = botlib;
  lua["jsonlib"] = jsonlib;
  load("./luarc");
  load("./user_luarc");
  lua["bot"]["_version"] = JIEMENG_VERSION;
  lua["bot"]["_platform"] = JIEMENG_PLATFORM;
  lua["bot"]["_compile_time"] = UPDATE_TIME;
  lua["bot"]["spliter"] = bot->config.spliter;
  auto custom_config_table = json_to_lua_table(bot->config.custom_config, lua);
  lua["bot"]["custom_config"] = custom_config_table;
}
string Lua_Shell::call(const string &func, Message &message)
{
  std::lock_guard<std::mutex> locker(mtx);
  debug_lable("[Lua_Call]");
  dout << "call: " << func << "\n";
  try
  {
    auto fun = lua.script("return "s + func);
    sol::function fu = fun;
    auto result = fu(message);
    // auto result = lua[func](message);
    if (!result.valid())
      throw sol::error(result);
    string q = lua["tostring"](result);
    return q;
  }
  catch (const sol::error &e)
  {
    JM_EXCEPTION("[Lua_Call]")
    return "";
  }
}
string Lua_Shell::exec(const string &code)
{
  std::lock_guard<std::mutex> locker(mtx);
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
nlohmann::json lua_table_to_json(sol::object lua_value)
{
  if (lua_value.is<bool>())
  {
    return lua_value.as<bool>();
  }
  else if (lua_value.is<int>() || lua_value.is<float>() || lua_value.is<double>())
  {
    return lua_value.as<double>();
  }
  else if (lua_value.is<std::string>())
  {
    return lua_value.as<std::string>();
  }
  else if (lua_value.is<sol::table>())
  {
    auto table = lua_value.as<sol::table>();
    auto first_element = *table.begin();
    if (first_element.first.is<int>())
    { // Check if the table is an array
      nlohmann::json json_array = nlohmann::json::array();
      for (const auto &pair : table)
      {
        json_array.push_back(lua_table_to_json(pair.second));
      }
      return json_array;
    }
    else
    {
      // It's an object, we assume keys are strings here
      nlohmann::json json_object;
      for (const auto &pair : table)
      {
        json_object[pair.first.as<std::string>()] = lua_table_to_json(pair.second);
      }
      return json_object;
    }
  }
  else
  {
    return nullptr;
  }
}
sol::object json_to_lua_table(const nlohmann::json &j, sol::state &lua)
{
  if (j.is_boolean())
  {
    return sol::make_object(lua, j.get<bool>());
  }
  else if (j.is_number())
  {
    return sol::make_object(lua, j.get<double>());
  }
  else if (j.is_string())
  {
    return sol::make_object(lua, j.get<std::string>());
  }
  else if (j.is_array())
  {
    // 创建一个新的数组（Lua表）
    sol::table lua_table = lua.create_table();
    int index = 0;
    for (const auto &item : j)
    {
      lua_table[++index] = json_to_lua_table(item, lua);
    }
    return lua_table;
  }
  else if (j.is_object())
  {
    // 创建一个新的Lua表
    sol::table lua_table = lua.create_table();
    for (auto &[key, value] : j.items())
    {
      lua_table[key] = json_to_lua_table(value, lua);
    }
    return lua_table;
  }
  else
  { // null 或其他类型
    return sol::make_object(lua, sol::nil);
  }
}