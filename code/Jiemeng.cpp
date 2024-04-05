#include "Jiemeng.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include "Jiemeng_Message.hpp"
#include "Jiemeng_DebugIO.hpp"
#include <nlohmann/json.hpp>
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Operation.hpp"
using namespace std;

void work_dir_check()
{
}
void Jiemeng::init()
{
  config_init();
  lua_init();
  deck_init();
  answer_init();
  server_init();
}

void Jiemeng::process_message(Message message)
{
  try
  {
    Operation_List list = answer.get_list(message);
    list.upgrade(message);
    for (auto i : list.list)
    {
      exec_operation(message, i);
    }
  }
  catch (Not_Serious)
  {
  }
}
using Type = Operation::Type;
void Jiemeng::exec_operation(Message &message, const Operation &operation)
{
  if (operation.type == Type::message)
  {
    CQMessage ms(operation.str);
    message_output(message.place, ms);
  }
  else if (operation.type == Type::lua_call)
  {
    lua.call(operation.str, message);
  }
  else if (operation.type == Type::lua_shell)
  {
    CQMessage ms(lua.exec(operation.str));
    message_output(message.place, ms);
  }
}
void Jiemeng::lua_init()
{
  lua.init(this);
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
  js = server.ws_send(js);
  return js["data"]["group_name"];
}