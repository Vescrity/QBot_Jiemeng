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
#include <nlohmann/json.hpp>
#include "urls.h"
#include "Get_Base_64.h"
#include "Module_Draw.h"
using json = nlohmann::json;
using namespace std;

string Order(const Message_Type &type, const string &order, const json &js)
{

  const string &msg = type.btype.message;
  try
  {
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
    else if (order == "AI_img2img")
    {
      AI_draw.start_img2img(type.btype, js);
      return "";
    }
    else if (order == "formPost")
    {
      int para_num = 0;
      vector<string> para;
      para_num = js["para_num"];
      Get_Para(msg.substr(msg.find("#") + 1), para_num, &para);
      string form = js["form"];
      return Basic_String_Replace(
          "{#", "}", form,
          [&para](const string &num)
          {
            int n;
            sscanf(num.c_str(), "%d", &n);
            if (para.size() < n)
              throw std::out_of_range("invalid n given");
            return para[n - 1];
          });
    }

    throw invalid_argument("Undefined Order "s + order);
  }
  catch (const std::exception &e)
  {
    std::string msg = "Exception caught: ";
    msg += e.what();
    error_lable("[Order]");
    error_puts(msg.c_str());
  }
  return "";
}
string Order(const Message_Type &type, const string &order, const string &Para_list)
{
  string para_list = Para_list;
  para_list = str_strchg("&#91;", "[", para_list.c_str());
  para_list = str_strchg("&#93;", "]", para_list.c_str());
  para_list = str_strchg("&amp;", "&", para_list.c_str());
  para_list = Basic_Message_Change(type, para_list);
  try
  {
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
    else if (order == "reRecv")
      return reRecv(type, para_list);
    else if (order == "rePost")
      return para_list;
    else if (order == "run_cmd")
    {
      return execmd(para_list);
    }
    else if (order == "type_reRecv" || order == "type_rePost")
    {
      vector<string> paras;
      Get_Para(para_list, 3, &paras);
      Basic_Message_Type btype;
      btype.admin_level = 100;
      btype.group_id = paras[0];
      btype.user_id = paras[1];
      // btype.message = paras[2];
      btype.ifgroup = ((btype.group_id == "0") ? 0 : 1);
      order == "type_reRecv" ? type_reRecv(btype, paras[2]) : type_rePost(btype, paras[2]);
      return "";
    }
#ifdef JIEMENG_DECK
    else if (order == "draw_deck")
    {
      vector<string> paras;
      try
      {
        Get_Para(para_list, 2, &paras, " ");
      }
      catch (std::runtime_error &e)
      {
        return decks.draw(para_list);
      }
      int n = stoi(paras[1]);
      return decks.draw(paras[0], n);
    }
    else if (order == "deck_reload")
    {
      decks.init();
      return "Deck 模块重裁完成！";
    }
    else if (order == "ls_deck")
    {
      return decks.list();
    }
#endif
    else if (order == "del_msg")
    {
      Message_Delete_Order(type.btype.message);
      return "";
    }
    else if (order == "AI_draw")
    {
      try
      {
        AI_draw.task(type.btype);
      }
      catch (std::exception &e)
      {
        JM_EXCEPTION("[AI_Draw]")
        return e.what();
      }

      return "";
    }

    else if (order == "cpp_run")
    {
      return cpp_runner(para_list);
    }
    else if (order == "mat_run")
    {
      return Matlab_runner(para_list);
    }
    else if (order == "py_run")
    {
      return py_runner(para_list);
    }
    else if (order == "r_run")
    {
      return R_runner(para_list);
    }
    else if (order == "fortran_run")
    {
      return fortran_runner(para_list);
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
      if (!para_list.length())
        throw invalid_argument("Order Ungiven");
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
    else if (order == "Baidu_Trans")
    {
      string id, sc;
      js_getval(id, configs.custom_config, "Baidu_Appid");
      js_getval(sc, configs.custom_config, "Baidu_Appsecret");
      baidu_trans.set(id, sc);
      return baidu_trans.operate(para_list);
    }
    else if (order == "7d_Weather")
    {
      Request *rq = new Request;
      rq->set_url("https://v2.alapi.cn/api");
      rq->set_api("/weather/forecast");
      rq->add_Headers("Content-Type: application/json");
      rq->set_msgs(R"({"location":")"s + para_list + R"(","token":")" + string(configs.custom_config["AL_TOKEN"]) + "\"}");
      string rt = rq->Post();
      City city;
      city.city_7d_init(rt.c_str());
      rt.clear();
      rt = city._7dprint();
      delete rq;
      if (configs.custom_config.count("IF_HAVE_MATLAB"))
        if (configs.custom_config["IF_HAVE_MATLAB"])
        {
          city.get_7dplot();
          return rt + "[-cut-]" + city.outplot();
        }

      return rt;
    }
    else if (order == "24h_Weather")
    {
      Request *rq = new Request;
      rq->set_url("https://v2.alapi.cn/api");
      rq->set_api("/weather/hourly");
      rq->add_Headers("Content-Type: application/json");
      rq->set_msgs(R"({"location":")"s + para_list + R"(","token":")" + string(configs.custom_config["AL_TOKEN"]) + "\"}");
      string rt = rq->Post();
      City city;
      city.city_24h_init(rt.c_str());
      rt.clear();
      rt = city._24hprint();
      delete rq;
      if (configs.custom_config.count("IF_HAVE_MATLAB"))
        if (configs.custom_config["IF_HAVE_MATLAB"])
        {
          city.get_24hplot();
          return rt + "[-cut-]" + city.outplot();
        }

      return rt;
    }
    else if (order == "Restart")
      exit(1);

    throw invalid_argument("Undefined Order: "s + order);
  }
  catch (std::exception &e)
  {
    JM_EXCEPTION("[Order]")
    return "";
  }

  return "";
}

#endif