#include <future>
#include <fstream>
#include "Jiemeng_Deck.hpp"
#include "Jiemeng.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Socket.hpp"
using namespace std;
void Jiemeng::init()
{
  int st = clock();
  debug_lable("[INIT]");
  debug_puts("开始执行 Config_init");
  config_init();
  auto fd = async(launch::async, []
                  { return new Deck; });
  auto fans = async(launch::async, [this]
                    { answer_init(); });
  debug_lable("[INIT]");
  debug_puts("开始执行 lua_init");
  lua_init();
  debug_lable("[INIT]");
  debug_puts("开始执行 server_init");
  server_init();
  fans.get();
  deck = fd.get();
  char *buf = new char[1 << 10];
  sprintf(buf, "初始化成功！共耗时%.3lfms", (clock() - st) * 1000.0 / CLOCKS_PER_SEC);
  info_lable("[INIT]");
  info_puts(buf);
  delete[] buf;
}

void Jiemeng::deck_reload()
{
  Deck *p = new Deck;
  Deck *r = deck;
  deck = p;
  delete r;
}

void Jiemeng::answer_init()
{
  debug_lable("[INIT]");
  debug_puts("开始初始化应答库");
  answer.init(config.custom_config);
  debug_lable("[INIT]");
  debug_puts("应答库初始化成功！");
}
void Jiemeng::answer_reload()
{
  answer.main_answer_reload(config.custom_config);
}

void Jiemeng::config_init()
{
  ifstream fread("config.json");
  config.init(json::parse(fread));
}
void Jiemeng::lua_init()
{
  lua = new Lua_Shell(this);
  for (auto &state : config.lua_state_list)
  {
    auto s = map_lua[state["name"]] = new Lua_Shell(this);
    s->load(state["path"]);
  }
}
void Jiemeng::server_init()
{
  server = new Server;
  server->init("127.0.0.1", config.port);
}
