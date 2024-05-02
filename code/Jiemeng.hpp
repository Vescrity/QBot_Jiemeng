#ifndef JIEMENG_HPP
#define JIEMENG_HPP

#include "Jiemeng_Config.hpp"
#include "Jiemeng_Answer.hpp"
#include <map>
#include <string>
using namespace std;
class Operation;
class Server;
class Lua_Shell;
class Deck;
class Jiemeng
{
  Deck *deck;
  All_Answer answer;
  Config config;
  Server *server;
  Lua_Shell *lua;
  map<string, Lua_Shell *> map_lua;
  Message generate_message(const json &);
  void process_message(Message);
  bool message_output(const Message &);
  bool message_output(const Message_Place &, const CQMessage &);
  bool group_output(const string &group_id, const CQMessage &message);
  bool private_output(const string &user_id, const CQMessage &message);
  bool is_white(const Message_Place &);
  string get_group_name(const string &group_id);
  string get_user_name(const string &group_id, const string &user_id);
  string get_user_name(const string &user_id);
  string exec_operation(const Message &, const Operation &);

  void answer_init();
  void config_init();
  void server_init();
  /// @brief 仅限初始化使用！
  void lua_init();
  void answer_reload();
  void deck_reload();
  void save_config();
  void process_operation(const Message &, Operation_List &);
  void process_operation(const Message &, Operation_List &, string &);
  friend class Lua_Shell;
  friend Operation_List extract(Operation oper, const Message &message, Jiemeng *bot);

public:
  Jiemeng();
  ~Jiemeng();
  void init();
  void clear();
  void run();
  json ws_send(json &a);
};

void work_dir_check();
#endif
