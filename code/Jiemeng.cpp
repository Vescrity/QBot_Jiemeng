#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include "Jiemeng_Message.hpp"
#include "Jiemeng_DebugIO.hpp"
#include <nlohmann/json.hpp>
#include "Jiemeng_Algorithm.hpp"
using namespace std;

void work_dir_check()
{
}
void Main_Task(const json &js);
void Jiemeng::run()
{
  std::thread([this]
              { server.run(); })
      .detach();
  while (1)
  {
    Main_Task(server.get_message());
  }
}

void Jiemeng::deck_init()
{
}
void Jiemeng::config_init()
{
  ifstream fread("config.json");
  config.init(json::parse(fread));
}
void Jiemeng::answer_init()
{
}

void Main_Task(const json &js)
{
  dout << js.dump(2) << "\n";
  const string &post_type = js["post_type"];
  string able_type[] = {"message", "message_sent", "notice"};
  if (!array_search(post_type, able_type))
  {
    debug_lable("[Event]");
    debug_puts("Not a message.");
  }
  else
  {
    Message message;
    message.init(js);
    message.show();
  }
}
