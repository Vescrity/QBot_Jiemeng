#ifndef JIEMENG_PLUGIN
#define JIEMENG_PLUGIN
#include <string>
#include "json.hpp"
#include "Message_Type.h"
using string = std::string;
using json = nlohmann::json;
/// @brief Run the plugin given
/// @param type The message
/// @param plugin_name The plugin's name
/// @param is_py if the plugin is a python plugin
/// @return the result of the plugin
string run_plugin(const Message_Type &type, const string &plugin_name, bool is_py)
{

  info_lable("[plugin]");
  info_print("调用插件");
  info_puts(plugin_name.c_str());
  string commd;
  if (is_py)
  {
    commd = "python ./plugin/";
    commd = commd + plugin_name + "/" + plugin_name + ".py";
  }
  else
  {
#ifdef LINUX_V
    commd = "./plugin/";
    commd = commd + plugin_name + "/" + plugin_name;
#else
    commd = ".\\plugin\\";
    commd = commd + plugin_name + "\\" + plugin_name + ".exe";
#endif
  }
  FILE *fp;
  fp = fopen("Current_msg.txt", "w");
  fprintf(fp, "%s", type.btype.message.c_str());
  fclose(fp);
  using json = nlohmann::json;
  json Msg_dta;
  Msg_dta["user_id"] = type.btype.user_id;
  Msg_dta["user_name"] = type.user_name;
  if (type.btype.ifgroup)
  {
    Msg_dta["group_id"] = type.btype.group_id;
    Msg_dta["group_name"] = type.group_name;
  }
  Msg_dta["message"] = type.btype.message;
  string str = to_string(Msg_dta);
  fp = fopen("Message_type.txt", "w");
  fprintf(fp, "%s", str.c_str());
  fclose(fp);
  return execmd(commd);
}

#endif