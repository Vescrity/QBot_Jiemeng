#ifndef JIEMENG_CONFIG
#define JIEMENG_CONFIG
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "Jiemeng_Algorithm.hpp"
using namespace std;
using json=nlohmann::json;
class Config
{
  vector<string> private_list;
  vector<string> black_list;
  vector<string> group_list;
  vector<string> broad_list;
  json admin_list;
  bool is_private_black;
public:
  string port;
  string self_id;
  string spliter;
  int text_length;
  int time_check;
  json lua_state_list;
  void add_group_list     (const string &id) { group_list.push_back(id); }
  void add_private_list   (const string &id) { private_list.push_back(id); }
  void add_black_list     (const string &id) { black_list.push_back(id); }
  void del_group_list     (const string &id) { vec_remove_first_value(group_list, id); }
  void del_private_list   (const string &id) { vec_remove_first_value(private_list, id); }
  void del_black_list     (const string &id) { vec_remove_first_value(black_list, id); }
  bool private_white      (const string &id) { return is_private_black ^ Vec_Find(private_list, id); }
  bool group_white        (const string &id) { return Vec_Find(group_list, id); }
  bool is_black           (const string &id) { return Vec_Find(black_list, id); }
  int  get_admin_level    (const string &id) { return admin_list.count(id) ? int(admin_list[id]) : 0; }
  json custom_config;

  void init(const json&);

};
#endif
