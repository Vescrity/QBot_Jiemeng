#include <thread>
#include <future>
#include <chrono>
#include <fstream>
#include "Jiemeng.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_DebugIO.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Time.hpp"
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;
void Jiemeng::run()
{
  std::thread(
      [this]
      { 
        string ti,tp;
        for(;;)
        {
          Time_Class tm;
          ti=tm.time_mark();
          if(ti!=tp)
          {
            tp=ti;
            Message msg(ti);
            msg.user_id="10000";
            msg.user_nm="时钟消息";
            msg.set_private();
            std::thread([this, msg]
                  { this->process_message(msg); })
            .detach();
          }
          minisleep(config.time_check);
        } })
      .detach();

  auto pmsg = [this](const json &js)
  {
    try
    {
      Message msg = generate_message(js);
      msg.show();
      process_message(msg);
    }
    catch (const Not_Serious &)
    {
      return;
    }
  };
  server->run(pmsg);
}

void Jiemeng::save_config()
{
  FILE *fp;
  fp = fopen("config.json", "w");
  fprintf(fp, "%s", config.save().dump(2).c_str());
  fclose(fp);
}

bool dir_exists(const string &name)
{
  if (fs::exists(name) && fs::is_directory(name))
    return 1;
  return 0;
}
bool file_exists(const string &name)
{
  if (fs::exists(name))
    return 1;
  return 0;
}

void work_dir_check()
{
  if (!dir_exists("tmp"))
  {
    if (fs::create_directory("tmp"))
      ;
    else
      throw runtime_error("Failed to creat dir tmp.");
  }
  if (!dir_exists("txt2img"))
  {
    warn_lable("[txt2img]");
    warn_puts("没有发现 txt2img 文件夹，请确保运行时不会触发相关功能。");
  }
  // TODO
  return;
}

Jiemeng::Jiemeng() { init(); }
Jiemeng::~Jiemeng() { clear(); }
void Jiemeng::clear()
{
  delete server;
  delete lua;
  delete deck;
  debug_lable("[Clear]");
  debug_puts("清理完毕。");
}

json Jiemeng::ws_send(json &a) { return server->ws_send(a); }
void Jiemeng::process_operation(const Message &message, Operation_List &list, string &buf)
{
  list.upgrade(message, this);
  for (auto &i : list.list)
  {
    try
    {
      buf = buf + exec_operation(message, i);
    }
    catch (const Operation::Clear &)
    {
      CQMessage ms(buf);
      message_output(message, ms);
      buf = "";
    }
    catch (Operation::reRecv &e)
    {
      Message msg = message;
      msg.change(string(e.what()));
      Operation_List rl;
      rl = answer.get_list(msg);
      process_operation(msg, rl, buf);
    }
    catch (exception &e)
    {
      JM_EXCEPTION("[Exec_Operation]");
    }
  }
}
void Jiemeng::process_operation(const Message &message, Operation_List &list)
{
  string uwu = "";
  Operation clears;
  clears.type = Operation::Type::clear;
  list += clears;
  process_operation(message, list, uwu);
}
void Jiemeng::process_message(Message message)
{
  try
  {
    Operation_List list = answer.get_list(message);
    process_operation(message, list);
  }
  catch (const Not_Serious &)
  {
  }
}

Message Jiemeng::generate_message(const json &js)
{
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
    message.get_level(&config);
    if (message.is_group())
      message.group_nm = get_group_name(message.group_id);
    return message;
  }
  throw Not_Serious();
}

string Jiemeng::get_group_name(const string &group_id)
{
  json js;
  js["params"]["group_id"] = stoull(group_id);
  js["action"] = "get_group_info";
  auto result = async(launch::async, [this, &js]
                      { return server->ws_send(js); });
  if (result.wait_for(chrono::milliseconds(config.wait_time)) == future_status::timeout)
  {
    warn_lable("[Get_Group_Name]");
    warn_puts("获取群名超时，放弃等待。");
    return "";
  }
  js = result.get();
  if (js["data"].is_null())
    return "";
  else if (!js["data"].contains("group_name"))
    return "";
  js = js["data"]["group_name"];
  if (js.is_null())
    return "";
  return js;
}
