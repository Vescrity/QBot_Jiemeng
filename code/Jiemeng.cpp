#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include "Jiemeng_Message.hpp"
using namespace std;

void work_dir_check()
{
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
  const json &post_type = js["post_type"];
  if (post_type != "message" && post_type != "message_sent")
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

/// @brief Process message and creat a task thread to handle it.
void ProcessMessage(const std::string &message)
{
  json ev = json::parse(message);
  // raw_generate(ev);
  std::thread t(Main_Task, ev);
  t.detach();
}
