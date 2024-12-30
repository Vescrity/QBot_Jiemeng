#include <filesystem>
#include <thread>
#include "Jiemeng_Random.hpp"
#include "txt2img_api.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng.hpp"
#include "Jiemeng_Version.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_MessageReplace.hpp"
#include "Jiemeng_Request.hpp"
#include "Jiemeng_Status.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;
json lua_table_to_json(sol::object lua_value);
sol::object json_to_lua_table(const nlohmann::json &j, sol::state &lua);
json parse_to_json(const sol::object &obj);
void Lua_Shell::load(const string &path)
{
  for (const auto &entry : fs::directory_iterator(path))
  {
    if (entry.path().extension() == ".lua")
    {
      try
      {
        lua->script_file(entry.path().string());
        debug_lable("[Lua_Load]");
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
void Lua_Shell::reload()
{
  std::lock_guard<std::mutex> locker(mtx);
  lua.reset();
  lua = std::make_unique<sol::state>();
  init(bot);
  return;
}
void Lua_Shell::init(Jiemeng *b)
{
  lua->open_libraries();
  if (bot == nullptr) bot = b;

  auto json_index = [](nlohmann::json &j, sol::this_state s, const std::string &key) -> sol::object
  {
    sol::state_view lua(s);
    if (j.contains(key))
    {
      return sol::make_object(lua, j[key]);
    }
    else
    {
      return sol::nil;
    }
  };
  lua->new_usertype<json>(
      "json",
      "new", sol::constructors<json()>(),
      "dump",
      sol::overload(
          [](json &js)
          { return js.dump(); },
          [](json &js, int n)
          { return js.dump(n); }),
      "__index", json_index,
      // __newindex metamethod，设置子键
      "__newindex", [](nlohmann::json &this_json, const std::string &key, const sol::stack_object &value)
      { this_json[key] = value.as<nlohmann::json>(); },
      "val", [&](json &js) -> sol::object
      { return json_to_lua_table(js, *lua); },
      "parse", [&](json &js, const sol::object &obj)
      { js = parse_to_json(obj); });
  lua->new_usertype<Request>(
      "Request",
      "new", sol::constructors<Request()>(),
      "add_Headers", &Request::add_Headers,
      "set_url", &Request::set_url,
      "set_api", &Request::set_api,
      "set_data", &Request::set_data,
      "set_msgs", &Request::set_msgs,
      "Post", &Request::Post,
      "js_post", &Request::js_post,
      "js_get", &Request::js_get,
      "Get", &Request::Get);
  lua->new_usertype<Message_Place>(
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
      "is_private", &Message_Place::is_private,
      "get_level",
      [b](Message_Place &place)
      { place.get_level(&(b->config)); });
  lua->new_usertype<Message>(
      "Message",
      "new", sol::constructors<Message()>(),
      "get_string", &Message::get_string,
      "get_json", &Message::get_json,
      "get_level",
      [b](Message &place)
      { place.get_level(&(b->config));
        return place.level; },
      "get_place", &Message::place,
      "true_str", &Message::true_str,
      "show", &Message::show,
      "is_group", &Message::is_group,
      "is_private", &Message::is_private,
      "change", sol::overload([](Message &m, const char *str)
                              { return m.change(str); },
                              [](Message &m, json &js)
                              { return m.change(js); }),
      "set_group", &Message::set_group,
      "set_private", &Message::set_private,
      "user_id", &Message::user_id,
      "user_nm", &Message::user_nm,
      "group_id", &Message::group_id,
      "group_nm", &Message::group_nm,
      "level", &Message::level);
  lua->new_usertype<Operation>(
      "Operation",
      "new", sol::constructors<Operation()>(),
      "str", &Operation::str,
      "set_type", &Operation::set_type);
  lua->new_usertype<Operation_List>(
      "Operation_List",
      "new", sol::constructors<Operation_List()>(),
      "push_back", &Operation_List::push_back);
  sol::table botlib = lua->create_table();
  sol::table jsonlib = lua->create_table();
  botlib.set_function("sleep", minisleep);
  botlib.set_function("rand", Rands);
  botlib.set_function("change_Rcode", chg_Rcode);
  botlib.set_function("string_only", string2CQ);
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
      "draw_deck",
      [this](const string key)
      { return this->bot->deck->draw(key); });
  botlib.set_function(
      "deck_list",
      [this]()
      { return this->bot->deck->list(); });
  botlib.set_function(
      "message_replace",
      [this](const string str, Message &message)
      {
        string s = str;
        message_replace(s, message);
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
  botlib.set_function(
      "lua_reload",
      [this]()
      { 
        thread([this]{reload();}).detach();
        return ""; });

  botlib.set_function(
      "ws_send",
      [this](const sol::object &obj)
      {
        auto a = parse_to_json(obj);
        return this->bot->ws_send(a);
      });
  botlib.set_function(
      "ws_send_async",
      [this](const sol::object &obj)
      { 
        json a = parse_to_json(obj);
        thread(
            [this,a]
            { json b=a;this->bot->ws_send(b); })
            .detach(); });
  botlib.set_function("start_up_time", start_up_time);
  botlib.set_function("txt2img", txt2img);
  botlib.set_function(
      "get_deck_size",
      [this]
      { return this->bot->deck->size(); });
  botlib.set_function(
      "get_answer_size",
      [this]
      { return this->bot->answer.main_answer->size(); });
  // Config
  botlib.set_function(
      "get_group_list",
      [this]
      { return bot->config.get_group_list(); });
  botlib.set_function(
      "add_group_list",
      [this](const string &str)
      { this->bot->config.add_group_list(str); });
  botlib.set_function(
      "add_private_list",
      [this](const string &str)
      { this->bot->config.add_private_list(str); });
  botlib.set_function(
      "add_black_list",
      [this](const string &str)
      { this->bot->config.add_black_list(str); });
  botlib.set_function(
      "del_private_list",
      [this](const string &str)
      { this->bot->config.del_private_list(str); });
  botlib.set_function(
      "del_group_list",
      [this](const string &str)
      { this->bot->config.del_group_list(str); });
  botlib.set_function(
      "del_black_list",
      [this](const string &str)
      { this->bot->config.del_black_list(str); });
  botlib.set_function(
      "save_config",
      [this]()
      { this->bot->save_config(); });
  botlib.set_function(
      "state_run",
      [this](const string &state_name, const string &code)
      { return this->bot->map_lua[state_name]->exec(code); });
  botlib.set_function(
      "state_run_async",
      [this](const string &state_name, const string &code)
      {
        string n = state_name;
        string c = code;
        thread([this, n, c]
               { this->bot->map_lua[n]->exec(c); })
            .detach();
      });
  jsonlib.set_function(
      "table2json",
      lua_table_to_json);
  jsonlib.set_function(
      "json2table",
      [&](json &js)
      { return json_to_lua_table(js, *lua); });

  (*lua)["bot"] = botlib;
  (*lua)["jsonlib"] = jsonlib;
  (*lua)["bot"]["_version"] = JIEMENG_VERSION;
  (*lua)["bot"]["_platform"] = JIEMENG_PLATFORM;
  (*lua)["bot"]["_compile_time"] = UPDATE_TIME;
  (*lua)["bot"]["spliter"] = bot->config.spliter;
  (*lua)["bot"]["custom_config"] = json_to_lua_table(bot->config.custom_config, *lua);
  (*lua)["bot"]["group_list"] = bot->config.get_group_list();
  lua->script_file("./init.lua");
  //load("./luarc");
  //load("./user_luarc");
}

sol::protected_function_result Lua_Shell::get_func(const string &func)
{
  // bool l = mtx.try_lock();
  auto fun = lua->script("return "s + func);
  // if (l)
  // mtx.unlock();
  return fun;
}

string Lua_Shell::call(const string &func, Message message)
{
  std::lock_guard<std::mutex> locker(mtx);
  debug_lable("[Lua_Call]");
  dout << "call: " << func << "\n";
  try
  {
    auto fun = lua->script("return "s + func);
    sol::function fu = fun;
    auto result = fu(message);
    if (!result.valid())
      throw sol::error(result);
    string q = (*lua)["tostring"](result);
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
    str = lua->script(code);
  }
  catch (const sol::error &e)
  {
    JM_EXCEPTION("[Lua_Call]")
    return "";
  }
  return str;
}
