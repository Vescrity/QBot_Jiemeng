#include "Jiemeng_Config.hpp"

#include "Jiemeng_Socket.hpp"

class Jiemeng
{
  /*Deck deck;

  Answer_List answer;*/
  Config config;
  Server server;

public:
  void deck_init();
  void answer_init();
  void config_init();
  void server_init() { server.init("127.0.0.1", config.port); }
  void init()
  {
    config_init();
    deck_init();
    answer_init();
    server_init();
  }
  void run() { server.run(); }
  json ws_send(json &a) { server.ws_send(a); }
  
};

void work_dir_check();
