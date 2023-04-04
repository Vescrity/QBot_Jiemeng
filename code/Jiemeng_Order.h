#ifndef JIEMENG_ORDER
#define JIEMENG_ORDER
#include <string>
#include "Jiemeng_Algorithm.h"
#include "Jiemeng_Basic.h"
#include "File_Read.h"
#include "Message_Type.h"
#include "Code_Runner.h"
#include "Pre_Catch.h"
#include "Orders.h"
#include "json.hpp"
#include "urls.h"
#include "Module_Draw.h"
using json = nlohmann::json;
using namespace std;

string Order(const Message_Type &type, const string &order, const json &js)
{
  const string &msg = type.btype.message;
  if (order == "run_api")
    return url_order(msg, order, js);
  else if (order == "pre_catch")
  {
    Pre_Catch *pc = new Pre_Catch;
    pc->type = type.btype;
    pc->init(js);
    pre_catch_list.push(pc);
    return "";
  }
  return "";
}
string Order(const Message_Type &type, const string &order, const string &Para_list)
{
  string para_list = Para_list;
  para_list = str_strchg("&#91;", "[", para_list.c_str());
  para_list = str_strchg("&#93;", "]", para_list.c_str());
  para_list = str_strchg("&#amp;", "&", para_list.c_str());
  para_list = string_format_with_json("{~", "}", configs.custom_config, para_list);

  if (order == "cut")
    return "[-cut-]";
  else if (order == "clear")
    return "[-clear-]";
  else if (order == "ignore")
    return "[IGNORE]";
  else if (order == "sleep")
  {
    int sltime;
    sscanf(para_list.c_str(), "%d", &sltime);
    minisleep(sltime);
    return "";
  }
  else if (order == "resend")
    return resend(type, para_list);
  else if (order == "repost")
    return para_list;
  else if (order == "run_cmd")
  {
    char buff[1 << 16] = {0};
    execmd(para_list.c_str(), buff);
    return buff;
  }
  else if (order == "type_resend" || order == "type_repost")
  {
    vector<string> paras;
    Get_Para(para_list, 3, &paras);
    Basic_Message_Type btype;
    btype.admin_level = 100;
    btype.group_id = paras[0];
    btype.user_id = paras[1];
    // btype.message = paras[2];
    btype.ifgroup = ((btype.group_id == "0") ? 0 : 1);
    order == "type_resend" ? type_resend(btype, paras[2]) : type_repost(btype, paras[2]);
    return "";
  }
  else if (order == "AI_draw")
  {
    try
    {
      AI_Draw(type.btype);
    }
    catch (std::exception &e)
    {
      exception_show("[AI_Draw]", e.what());
      return e.what();
    }

    return "";
  }
  else if (order == "cpp_run")
  {
    return cpp_runner(para_list);
  }
  else if (order == "py_run")
  {
    return py_runner(para_list);
  }
  else if (order == "r_run")
  {
    return R_runner(para_list);
  }
  else if (order == "txt2img")
  {
    return txt2img(para_list.c_str());
  }
  else if (order == "plugin")
    return run_plugin(type, para_list, 0);
  else if (order == "py_plugin")
    return run_plugin(type, para_list, 1);
  else if (order == "order")
  {
    size_t pos = para_list.find("#");
    return Order(type, para_list.substr(0, pos), para_list.substr(pos + 1));
  }
  else if (order == "Config_Reload")
  {
    Config_File_Read();
    return "";
  }
  else if (order == "Ans_Reload")
  {
    answers.clear();
    Ans_Read();
    return "";
  }
  else if (order.find("Add_") == 0 || order.find("Del_") == 0)
  {
    if (order == "Add_Grouplist")
      configs.list_config.add_group_list(para_list);
    else if (order == "Add_Private_list")
      configs.list_config.add_private_list(para_list);
    else if (order == "Add_Blacklist")
      configs.list_config.add_black_list(para_list);
    else if (order == "Del_Grouplist")
      configs.list_config.del_group_list(para_list);
    else if (order == "Del_Private_list")
      configs.list_config.del_private_list(para_list);
    else if (order == "Del_Blacklist")
      configs.list_config.del_black_list(para_list);

    return "";
  }

  //...
  try
  {
    throw invalid_argument("Undefined Order "s + order);
  }
  catch (std::exception &e)
  {
    std::string msg = "Exception caught: ";
    msg += e.what();
    error_lable("[Order]");
    error_puts(msg.c_str());
    minisleep(configs.sleep_time);
  }

  return "";
}

#endif