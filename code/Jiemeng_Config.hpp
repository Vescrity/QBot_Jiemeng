#ifndef JIEMENG_CONFIG
#define JIEMENG_CONFIG
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
using namespace std;
using json=nlohmann::json;
class Config
{

  
  vector<string> private_list;
  vector<string> black_list;
  vector<string> group_list;
  vector<string> broad_list;
public:
  string port;
  string self_id;
  //void add_group_list     (const string &id);
  //void add_private_list   (const string &id);
  //void add_black_list     (const string &id);
  //void del_group_list     (const string &id);
  //void del_private_list   (const string &id);
  //void del_black_list     (const string &id);
  //bool private_white      (const string &id);
  //bool group_white        (const string &id);
  //bool is_black           (const string &id);
  //int  get_admin_level    (const string &id);
  json custom_config;

  void init(const json&);

};
#endif
