#ifndef MODULE_DRAW
#define MODULE_DRAW
#include <string>
#include <stdio.h>
#include "opstring.h"
#include "Jiemeng_Config_Class.h"
#include "Message_Output.h"
#include "Get_Base_64.h"
#include <queue>

#include "urls.h"

using namespace std;

static int AI_TASK_NUM = 5;

static queue<Basic_Message_Type> tasklist;
static int reads(const char *buf, char *rt)
{
  int nxt;
  for (int i = 0;; i++)
  {
    if (buf[i] == '\n')
    {
      nxt = i + 1;
      rt[i] = 0;
      return nxt;
    }
    else if (buf[i] == 0)
    {
      rt[i] = 0;
      return -1;
    }
    rt[i] = buf[i];
  }
}

static bool crt_method(string method)
{
  return (method == "Euler a") || (method == "Euler") || (method == "DDIM") || (method == "LMS") || (method == "Heun") || (method == "DPM2") || (method == "DPM2 a") || (method == "DPM++ 2S a") || (method == "DPM++ 2M") || (method == "DPM++ SDE") || (method == "DPM fast") || (method == "DPM adaptive") || (method == "DPM2 Karras") || (method == "DPM2 a Karras") || (method == "DPM++ 2S Karras") || (method == "DPM++ 2M Karras") || (method == "DPM++ SDE Karras") || (method == "PLMS");
}

class AI_Para
{
  string x, y, step, strength, prompt, method;

  bool check() const
  {
    int lx, ly, ls, lstr;
    sscanf(x.c_str(), "%d", &lx);
    sscanf(y.c_str(), "%d", &ly);
    sscanf(step.c_str(), "%d", &ls);
    sscanf(strength.c_str(), "%d", &lstr);
    if (lx < 1 || ly < 1 || ls < 1 || (is_img2img && (lstr < 1 || lstr > 99)) || (lx & 63) || (ly & 63) || ls > 50 || (lx * ly * ls > 1920 * 1088 * 30) || (!crt_method(method)))
      return 0;
    return 1;
  }

public:
  bool is_img2img;
  string base;
  Basic_Message_Type type;
  AI_Para() { is_img2img = 0; }
  void init(const Basic_Message_Type &intype)
  {
    type = intype;
    const string &msg = intype.message;
    auto prompt_start = msg.find("#");
    if (prompt_start == string::npos)
      throw invalid_argument("invalid order");
    auto step_start = msg.find("%");
    auto size_start = msg.find("~");
    auto strength_start = msg.find("/");
    auto method_start = msg.find("**");
    auto pre_st = prompt_start;
    size_t tmp;
    prompt = msg.substr(prompt_start + 1);
    prompt = str_strchg("\r\n", "", prompt.c_str());
    prompt = str_strchg("\n", "", prompt.c_str());
    prompt = str_strchg("，", ", ", prompt.c_str());
    prompt = str_strchg("(", "(", prompt.c_str());
    prompt = str_strchg("）", ")", prompt.c_str());
    prompt = str_strchg("　", " ", prompt.c_str());
    if (method_start > pre_st)
      method = "Euler a";
    else
    {
      method = msg.substr(method_start + 2, pre_st - method_start - 2);
      pre_st = method_start;
    }
    if (strength_start < pre_st)
    {
      is_img2img = 1;
      strength = msg.substr(strength_start + 1, pre_st - strength_start - 1);
      pre_st = strength_start;
    }
    if (size_start > pre_st)
      x = y = "512";
    else
    {
      tmp = msg.substr(size_start + 1).find("~");
      if (tmp > pre_st - size_start)
        throw std::invalid_argument("invalid order");
      tmp = size_start + tmp + 1;
      size_start++;
      x = msg.substr(size_start, tmp - size_start);
      y = msg.substr(tmp + 1, pre_st - tmp - 1);
      pre_st = size_start - 1;
    }
    if (step_start > pre_st)
      step = "10";
    else
      step = msg.substr(step_start + 1, pre_st - step_start - 1);
    if (!check())
      throw std::invalid_argument("invalid argument in AI_order");
  }
  string AI_img2img();
  string AI_txt2img();
};

class AI_Wait
{
public:
  bool flg;
  AI_Para paras;
  AI_Wait() { flg = 0; }
};
class AI_Draw
{
  AI_Wait list[128];
  queue<int> tasks;
  bool running;
  int new_wait(const AI_Para &para)
  {
    for (int i = 0; i < 128; i++)
    {
      if (!list[i].flg)
      {
        list[i].flg = 1;
        list[i].paras = para;
        return i;
      }
    }
    throw std::out_of_range("Wait_List Full");
  }
  void operate(int num)
  {
    if (tasks.size() >= AI_TASK_NUM)
      throw runtime_error("Task queue full");
    tasks.push(num);
    if (running)
      return;
    running = 1;
    while (!tasks.empty())
    {
      int n = tasks.front();
      AI_Para &para = list[n].paras;
      Basic_Message_Output(para.type, para.is_img2img ? para.AI_img2img() : para.AI_txt2img());
      tasks.pop();
      list[n].flg = 0;
    }
    running = 0;
  }

public:
  AI_Draw() { running = 0; }
  void start_img2img(const Basic_Message_Type &type, int n)
  {
    list[n].paras.base = get_base64_from_message(type.message);
    operate(n);
  }
  void task(const Basic_Message_Type &type)
  {
    AI_Para paras;
    paras.init(type);
    if (paras.is_img2img)
    {
      Pre_Catch *pc = new Pre_Catch;
      pc->type = type;
      json js, sjs;
      js["regex"] = "image";
      sjs["order"] = "AI_img2img";
      sjs["data"] = new_wait(paras);
      js["anss"] = sjs;
      pc->init(js);
      pre_catch_list.push(pc);
      Basic_Message_Type ntype = type;
      type_reRecv(ntype, "[PIC_NEED]");
    }
    else
    {
      operate(new_wait(paras));
    }
  }
} AI_draw;

string AI_Para::AI_img2img()
{
  const string &a = x;
  const string &b = y;
  const string &Sample_step = step;
  char infos[1 << 14] = {0};
  info_lable("[AI_Gener]");
  sprintf(infos, "[x=%s][y=%s][step=%s][method=%s]\n[prompt=%s]", a.c_str(), b.c_str(), Sample_step.c_str(), method.c_str(), prompt.c_str());
  info_puts(infos);
  string req;
  string res;

  req = R"({"fn_index": 74,"data": [0,")"s + prompt +
        R"(","(worst quality, low quality:1.4), lowers, bad anatomy, bad hands, text, error, missing fingers, extra digits ,)"
        R"(fewer digits, cropped, wort quality ,low quality, normal quality, jpeg artifacts, )"
        R"(signature, watermark, username, blurry, bad feet","None","None","data:image/jpg;base64,)" +
        base +
        R"(",null,null,null,null,"Draw mask",)" + Sample_step + ",\"" + method + R"(",4,0,"original",false,false,1,1,7,0.)" + strength + R"(,-1,-1,0,0,0,false,)" + a + "," + b +
        R"(,"Just resize",false,32,"Inpaint masked","","","None","",true,true,"","",true,50,true,1,0,false,4,1,"",128,8,["left","right","up","down"],1,0.05,128,4,"fill",["left","right","up","down"],false,false,false,"","",64,"None","Seed","","Nothing","",true,false,false,null,"",""],"session_hash": ""})";
  Request *rq = new Request;
  rq->set_msgs(req);
  rq->set_api("/run/predict/");
  rq->set_url("http://127.0.0.1:7860");
  rq->add_Headers("Content-Type: application/json");
  string str = rq->Post();
  delete rq;
  string file_name = json_read(str.c_str(), "name");
  string fname = file_name.substr(file_name.find("\\") + 2);
  fname = url_encode(fname.c_str());
  file_name = file_name.substr(0, file_name.find("\\")) + "/" + fname;
  if (!configs.custom_config.count("AI_PATH"))
    return "AI_PATH not defined.";
  string path = file_path_chg(string(configs.custom_config["AI_PATH"]).c_str());
  return "[CQ:image,file="s + path + file_name + "]";
}

string AI_Para::AI_txt2img()
{
  const string &a = x;
  const string &b = y;
  const string &Sample_step = step;
  char infos[1 << 14] = {0};
  info_lable("[AI_Gener]");
  sprintf(infos, "[x=%s][y=%s][step=%s][method=%s]\n[prompt=%s]", a.c_str(), b.c_str(), Sample_step.c_str(), method.c_str(), prompt.c_str());
  info_puts(infos);
  string req;
  string res;

  req = "{\"fn_index\":51,\"data\":[\"" + prompt +
        "\",\"(worst quality, low quality:1.4), lowers, bad anatomy, bad hands, text, error, missing fingers, extra digits ,"
        "fewer digits, cropped, wort quality ,low quality, normal quality, jpeg artifacts, "
        "signature, watermark, username, blurry, bad feet\","
        "\"None\",\"None\"," +
        Sample_step + ",\"" + method + "\",false,false,1,1,7,-1,-1,0,0,0,false," +
        a + "," + b +
        ",false,0.7,0,0,\"None\",false,false,false,\"\",\"Seed\",\"\",\"Nothing\",\"\",true,false,false,null,\"\",\"\"],\"session_hash\":\"o8yial6116\"}";
  Request *rq = new Request;
  rq->set_msgs(req);
  rq->set_api("/run/predict/");
  rq->set_url("http://127.0.0.1:7860");
  rq->add_Headers("Content-Type: application/json");
  string str = rq->Post();
  delete rq;
  string file_name = json_read(str.c_str(), "name");
  string fname = file_name.substr(file_name.find("\\") + 2);
  fname = url_encode(fname.c_str());
  file_name = file_name.substr(0, file_name.find("\\")) + "/" + fname;
  if (!configs.custom_config.count("AI_PATH"))
    return "AI_PATH not defined.";
  string path = file_path_chg(string(configs.custom_config["AI_PATH"]).c_str());
  return "[CQ:image,file="s + path + file_name + "]";
}
#endif
