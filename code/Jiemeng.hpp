#ifndef JIEMENG_HPP
#define JIEMENG_HPP

#include "Jiemeng_Config.hpp"
#include "Jiemeng_Answer.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_Lua.hpp"
class Operation;

class Jiemeng
{
  /*Deck deck;*/
  All_Answer answer;
  Config config;
  Server server;
  Lua_Shell lua;
  Message generate_message(const json &);
  void process_message(Message);
  bool message_output(Message &);
  bool message_output(const Message_Place &, CQMessage &);
  bool message_output(const string &group_id, const string &user_id, CQMessage &message);
  bool message_output(const string &user_id, CQMessage &message);
  string get_group_name(const string &group_id);
  void exec_operation(Message &, const Operation &);
  void deck_init();
  void answer_init();
  void config_init();
  void server_init() { server.init("127.0.0.1", config.port); }
  void lua_init();
  friend class Lua_Shell;

public:
  void init();
  void run();
  json ws_send(json &a) { return server.ws_send(a); }
};

void work_dir_check();
#endif