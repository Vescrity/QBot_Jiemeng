#include "Jiemeng.hpp"
#include <fstream>
#include <thread>
#include <future>
#include <chrono>
#include "Jiemeng_Message.hpp"
#include "Jiemeng_DebugIO.hpp"
#include <nlohmann/json.hpp>
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Lua.hpp"
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

  auto pmsg = [this](const json &js)
  {
    Message msg;
    try
    {
      msg = generate_message(js);
    }
    catch (Not_Serious)
    {
      return;
    }
    msg.show();
    process_message(msg);
  };
  server->run(pmsg);
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

void Jiemeng::server_init()
{
  server = new Server;
  server->init("127.0.0.1", config.port);
}
json Jiemeng::ws_send(json &a) { return server->ws_send(a); }
void Jiemeng::process_operation(Message &message, Operation_List &list, string &buf)
{
  list.upgrade(message, this);
  for (auto &i : list.list)
  {
    try
    {
      buf = buf + exec_operation(message, i);
    }
    catch (Operation::Clear)
    {
      CQMessage ms(buf);
      message_output(message.place, ms);
      buf = "";
    }
    catch (Operation::reRecv &e)
    {
      Message msg;
      msg.change(e.what());
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
void Jiemeng::process_operation(Message &message, Operation_List &list)
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
  catch (Not_Serious)
  {
  }
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