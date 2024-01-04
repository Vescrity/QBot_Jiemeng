#ifndef JIEMENG_CONFIG_CLASS
#define JIEMENG_CONFIG_CLASS
#include "Jiemeng_Basic.h"
#include <nlohmann/json.hpp>
#include <string>
#define GOCQ_URL "http://127.0.0.1:"

using namespace std;
using json = nlohmann::json;

class List_Config
{
private:
  bool private_ifblack;

public:
  json admin_list;
  vector<string> private_list;
  vector<string> black_list;
  vector<string> group_list;
  vector<string> broad_list;
  void add_group_list(const string &id)   ;
  void add_private_list(const string &id) ;
  void add_black_list(const string &id)   ;
  void del_group_list(const string &id)   ;
  void del_private_list(const string &id) ;
  void del_black_list(const string &id)   ;
  bool private_white(const string &id)    ;
  bool group_white(const string &id)      ;
  bool is_black(const string &id)         ;
  int  get_admin_level(const string &id)  ;

  void init(const json &js);
  json save();
};
class All_Config
{
private:
  void config_init(const json &js);

public:
  int INPORT, OUTPORT, sleep_time, delay_time, Time_Check_Delay, MAX_TEXT_LENGTH, MAX_SINGLE_MESSAGE_LENGTH, MAX_MESSAGE_LENGTH, pswd;
  string Self_ID, SYMBOL_NAME, ANS_FILENAME, NOTE_FILENAME, TITLE, REPORT_ID, REPORT_GROUP;
  json custom_config;
  List_Config list_config;

  void init(const json &js)
  {
    config_init(js["Config"]);
    list_config.init(js["List_Config"]);
    custom_config = js["Custom_Config"];
  }
  json save();
} configs;

#endif
