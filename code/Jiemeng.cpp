#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include "Jiemeng_Message.hpp"
#include "Jiemeng_DebugIO.hpp"
#include <nlohmann/json.hpp>
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Time.hpp"
using namespace std;

void work_dir_check()
{
  // TODO
}

Jiemeng::Jiemeng()
{
  init();
}
Jiemeng::~Jiemeng()
{
  clear();
}
void Jiemeng::clear()
{
  delete server;
  delete lua;
  delete deck;
}

void Jiemeng::init()
{
  config_init();
  lua_init();
  deck_init();
  answer_init();
  server_init();
}

void Jiemeng::server_init()
{
  server = new Server;
  server->init("127.0.0.1", config.port);
}
json Jiemeng::ws_send(json &a) { return server->ws_send(a); }
void Jiemeng::process_message(Message message)
{
  try
  {
    Operation_List list = answer.get_list(message);
    list.upgrade(message);
    string str;
    for (auto i : list.list)
    {
      try
      {
        str = str + exec_operation(message, i);
      }
      catch (Operation::Clear)
      {
        CQMessage ms(str);
        message_output(message.place, ms);
        str = "";
      }
    }
  }
  catch (Not_Serious)
  {
  }
}

void Jiemeng::lua_init()
{
  lua = new Lua_Shell(this);
}
void Jiemeng::run()
{
  std::thread([this]
              { server->run(); })
      .detach();
  std::thread(
      [this]
      { 
        string ti,tp;
        for(;;)
        {
          Time_Class tm;
          Message msg;
          msg.place.user_id="10000";
          msg.place.user_nm="时钟消息";
          msg.place.set_private();
          ti=tm.time_mark();
          if(ti!=tp){
            tp=ti;
            msg.text.change(ti);
            std::thread([this, msg]
                  { this->process_message(msg); })
            .detach();
          }
          minisleep(config.time_check);
        } })
      .detach();
  while (1)
  {
    try
    {
      Message msg = generate_message(server->get_message());
      msg.show();
      std::thread([this, msg]
                  { this->process_message(msg); })
          .detach();
    }
    catch (Not_Serious)
    {
      continue;
    }
  }
}

void Jiemeng::answer_init()
{
  answer.init(config.custom_config);
}

void Jiemeng::deck_init()
{
  deck = new Deck;
}
void Jiemeng::config_init()
{
  ifstream fread("config.json");
  config.init(json::parse(fread));
}
Message Jiemeng::generate_message(const json &js)
{
  // dout << js.dump(2) << "\n";
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
    message.place.get_level(&config);
    if (message.is_group())
      message.place.group_nm = get_group_name(message.place.group_id);
    return message;
  }
  throw Not_Serious();
}

string Jiemeng::get_group_name(const string &group_id)
{
  json js;
  js["params"]["group_id"] = stoi(group_id);
  js["action"] = "get_group_info";
  js = server->ws_send(js);
  if (js["data"].is_null())
    return "";
  else if (!js["data"].contains("group_name"))
    return "";
  js = js["data"]["group_name"];
  if (js.is_null())
    return "";
  return js;
}