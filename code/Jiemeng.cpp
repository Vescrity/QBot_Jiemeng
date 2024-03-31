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

void Jiemeng::process_message(Message message)
{

}



void Jiemeng::run()
{
  std::thread([this]
              { server.run(); })
      .detach();
  while (1)
  {
    try
    {
      Message msg = generate_message(server.get_message());
      msg.show();
      std::thread([this,msg]{this->process_message(msg);}).detach();
    }
    catch (Not_Serious)
    {
      continue;
    }
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

Message Jiemeng::generate_message(const json &js)
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
    return message;
  }
  throw Not_Serious();
}
