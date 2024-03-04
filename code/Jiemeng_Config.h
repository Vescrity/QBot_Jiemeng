#ifndef JIEMENG_CONFIG
#define JIEMENG_CONFIG
#include <nlohmann/json.hpp>
#include "Jiemeng_Basic.h"
#include "Jiemeng_Algorithm.h"
#include "Jiemeng_Config_Class.h"
#include <string>

using json = nlohmann::json;
using namespace std;

/// @brief Try to extract the value from a JSON by the given key and store it in the given var
/// @tparam T The type of the given var
/// @param a The var to store the value
/// @param js The JSON contain the value
/// @param str The key to extract the value for
/// @throws std::invalid_argument if the given key cannot found in the given JSON
template <typename T>
void js_getval(T &a, const json &js, const string &str)
{
  if (!js.count(str))
    throw std::invalid_argument(str + " is not given (config)");

  else
    a = js[str];
}
/// @brief Extract the value from a JSON by the given key and store it in the given var
/// @tparam T The type of the given var
/// @param a The var to store the value
/// @param js The JSON contain the value
/// @param str The key to extract the value for
/// @param b If the given key cannot be found in the given JSON then set `a = b`
template <typename T>
void js_getval(T &a, const json &js, const string &str, const T &b)
{
  if (!js.count(str))
    a = b;
  else
  {
    try
    {
      a = js[str];
    }
    catch (const std::exception &e)
    {
      a = b;
      throw std::invalid_argument(string(e.what()) + "\n  at js_getval: str = " + str);
    }
  }
}

void  List_Config::add_group_list(const string &id)   { group_list.push_back(id); }
void  List_Config::add_private_list(const string &id) { private_list.push_back(id); }
void  List_Config::add_black_list(const string &id)   { black_list.push_back(id); }
void  List_Config::del_group_list(const string &id)   { vec_remove_first_value(group_list, id); }
void  List_Config::del_private_list(const string &id) { vec_remove_first_value(private_list, id); }
void  List_Config::del_black_list(const string &id)   { vec_remove_first_value(black_list, id); }
bool  List_Config::private_white(const string &id)    { return private_ifblack ^ Vec_Find(private_list, id); }
bool  List_Config::group_white(const string &id)      { return Vec_Find(group_list, id); }
bool  List_Config::is_black(const string &id)         { return Vec_Find(black_list, id); }
int   List_Config::get_admin_level(const string &id)  { return admin_list.count(id) ? int(admin_list[id]) : 0; }
void  List_Config::init(const json &js)
{
  try
  {
    private_ifblack = bool(js["private_ifblack"]);
    admin_list = js["admin_list"];
    private_list = js["private_list"].get<vector<string>>();
    black_list = js["black_list"].get<vector<string>>();
    group_list = js["group_list"].get<vector<string>>();
    if (js.count("broad_List"))
      broad_list = js["broad_list"].get<vector<string>>();
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Config]")
    exit(1);
  }
}
json List_Config::save()
{
  json js;
  js["private_ifblack"] = private_ifblack;
  js["private_list"]    = private_list;
  js["admin_list"]      = admin_list;
  js["black_list"]      = black_list;
  js["group_list"]      = group_list;
  js["broad_list"]      = broad_list;
  return js;
}

void All_Config::config_init(const json &js)
{
  try
  {
////js_getval(VAR,                        js, CONFIG_VAR,                   <DEFAULT_VALUE>     );
    js_getval(PORT,                       js, "PORT"                                            );
    js_getval(sleep_time,                 js, "sleep_time",                 50                  );
    js_getval(delay_time,                 js, "delay_time",                 2000                );
    js_getval(Time_Check_Delay,           js, "Time_Check_Delay",           30000               );
    js_getval(Self_ID,                    js, "Self_ID",                    "0"s                );
    js_getval(REPORT_ID,                  js, "REPORT_ID",                  "0"s                );
    js_getval(REPORT_GROUP,               js, "REPORT_GROUP",               "0"s                );
    js_getval(MAX_TEXT_LENGTH,            js, "MAX_TEXT_LENGTH",            1 << 10             );
    js_getval(MAX_SINGLE_MESSAGE_LENGTH,  js, "MAX_SINGLE_MESSAGE_LENGTH",  1 << 11             );
    js_getval(MAX_MESSAGE_LENGTH,         js, "MAX_MESSAGE_LENGTH",         1 << 14             );
    js_getval(TITLE,                      js, "TITLE",                      "Jiemeng"s + Self_ID);
    js_getval(pswd,                       js, "pswd",                       1234                );
    js_getval(Debug_Mode,                 js, "Debug_Mode",                 false               );
  }
  catch (const std::exception &e)
  {
    error_lable("[File_Read]");
    error_print("Error in Config: ");
    error_puts(e.what());
    exit(1);
  }
}
json All_Config::save()
{
  json ajs;
  json &js = ajs["Config"];
  js["PORT"]              = PORT;
  js["sleep_time"]        = sleep_time;
  js["delay_time"]        = delay_time;
  js["Time_Check_Delay"]  = Time_Check_Delay;
  js["Self_ID"]           = Self_ID;
  js["REPORT_ID"]         = REPORT_ID;
  js["REPORT_GROUP"]      = REPORT_GROUP;
  js["NOTE_FILENAME"]     = NOTE_FILENAME;
  js["NOTE_FILENAME"]     = NOTE_FILENAME;
  js["TITLE"]             = TITLE;

  ajs["Custom_Config"]    = custom_config;
  ajs["List_Config"]      = list_config.save();
  return ajs;
}

#endif
