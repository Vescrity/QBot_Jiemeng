#ifndef JIEMENG_HPP
#define JIEMENG_HPP

#include "Jiemeng_Config.hpp"

#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Message.hpp"

class Jiemeng
{
  /*Deck deck;

  Answer_List answer;*/
  Config config;
  Server server;
  Message generate_message(const json &);
  void process_message(Message);
  bool message_output(Message &);
  bool message_output(const Message_Place &, CQMessage &);
  bool message_output(const string &group_id, const string &user_id, CQMessage &message);
  bool message_output(const string &user_id, CQMessage &message);

  void deck_init();
  void answer_init();
  void config_init();
  void server_init() { server.init("127.0.0.1", config.port); }

public:
  void init()
  {
    config_init();
    deck_init();
    answer_init();
    server_init();
  }
  void run();
  json ws_send(json &a) { return server.ws_send(a); }
};

void work_dir_check();
#endif