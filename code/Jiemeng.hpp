#ifndef JIEMENG_HPP
#define JIEMENG_HPP

#include "Jiemeng_Config.hpp"
#include "Jiemeng_Answer.hpp"
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
  Message generate_message(const json &);
  void process_message(Message);
  bool message_output(Message &);
  bool message_output(const Message_Place &, CQMessage &);
  bool group_output(const string &group_id, CQMessage &message);
  bool private_output(const string &user_id, CQMessage &message);
  string get_group_name(const string &group_id);
  string exec_operation(Message &, const Operation &);
  
  void answer_init();
  void config_init();
  void server_init();
  /// @brief 仅限初始化使用！
  void lua_init();
  void answer_reload();
  void deck_reload();
  void process_operation(Message &, Operation_List &);
  friend class Lua_Shell;
  friend Operation_List extract(Operation oper, Message message, Jiemeng *bot);

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