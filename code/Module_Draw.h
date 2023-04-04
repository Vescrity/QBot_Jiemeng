#ifndef MODULE_DRAW
#define MODULE_DRAW
#define MAX_AI_NUM 6
#include <string>
#include "opstring.h"
#include "Message_Type.h"
#include "Message_Output.h"
#include <queue>

#include "urls.h"

using namespace std;

Basic_Message_Type types;

static queue<Basic_Message_Type> tasklist;
int reads(const char *buf, char *rt)
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

bool crt_method(string method)
{
  return (method == "Euler a") || (method == "Euler") || (method == "DDIM") || (method == "LMS") || (method == "Heun") || (method == "DPM2") || (method == "DPM2 a") || (method == "DPM++ 2S a") || (method == "DPM++ 2M") || (method == "DPM++ SDE") || (method == "DPM fast") || (method == "DPM adaptive") || (method == "DPM2 Karras") || (method == "DPM2 a Karras") || (method == "DPM++ 2S Karras") || (method == "DPM++ 2M Karras") || (method == "DPM++ SDE Karras") || (method == "PLMS");
}
string gener(string a, string b, string Sample_step, string prompt, string method)
{
  char infos[1 << 14] = {0};
  info_lable("[AI_Gener]");
  sprintf(infos, "[x=%s][y=%s][step=%s][method=%s]\n[prompt=%s]", a.c_str(), b.c_str(), Sample_step.c_str(), method.c_str(), prompt.c_str());
  info_puts(infos);
  string req;
  string res;
  req = "{\"fn_index\":51,\"data\":[\"" + prompt +
        "\",\"lowers, bad anatomy, bad hands, text, error, missing fingers, extra digits ,"
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
  string fname = file_name.substr(file_name.find("\\")+2);
  fname = url_encode(fname.c_str());
  file_name = file_name.substr(0, file_name.find("\\")) + "/" + fname;
  if (!configs.custom_config.count("AI_PATH"))
    return "AI_PATH not defined.";
  string path = file_path_chg(string(configs.custom_config["AI_PATH"]).c_str());
  return "[CQ:image,file="s + path + file_name + "]";
}
bool AI_Draw(Basic_Message_Type type)
{
  Basic_Message_Type crttask;
  crttask = type;

  int l, r;

  char ss[50] = {0};
  l = get_st(crttask.message.c_str(), "#");
  if (l == -1)
  {
    throw invalid_argument("No prompt given");
  }

  if (tasklist.size() > MAX_AI_NUM)
  {
    throw runtime_error("Task queue full");
  }
  if (tasklist.size())
  {
    tasklist.push(crttask);
    return 1;
  }
  tasklist.push(crttask);
  while (tasklist.size())
  {
    Basic_Message_Type now_task = tasklist.front();
    try
    {
      info_lable("[AI_Draw]");
      sprintf(ss, "当前任务队列长度：%d", tasklist.size());
      info_puts(ss);
      string l_a, l_b, l_step, l_method;

      // now_task.show();
      l = get_st(now_task.message.c_str(), "%");
      if (l != -1)
      {
        int aa, bb;
        char ttemp[8] = {0};
        aa = get_st(now_task.message.c_str(), "#");
        bb = get_st(now_task.message.c_str(), "~");
        r = bb == -1 ? aa : bb;
        if (r - l - 1 < 1)
        {
          throw invalid_argument("error");
        }
        get_copy(l + 1, r, now_task.message.c_str(), ttemp);
        for (int i = 0; i < strlen(ttemp); i++)
        {
          if (ttemp[i] < '0' || ttemp[i] > '9')
          {
            throw invalid_argument("not number");
          }
        }
        l_step = ttemp;

        l = r + 1;
      }
      else
        l_step = "10";
      l = get_st(now_task.message.c_str(), "**");

      if (l != -1)
      {
        r = get_st(now_task.message.c_str() + l + 3, "**") + l + 3;
        if (r - l - 3 == -1)
        {
          throw invalid_argument("invalid method");
        }
        char ttemp[25] = {0};
        get_copy(l + 2, r, now_task.message.c_str(), ttemp);
        l_method = ttemp;
        l = r + 1;
      }
      else
        l_method = "Euler a";
      l = get_st(now_task.message.c_str(), "~");

      if (l == -1)
      {
        l_a = l_b = "512";
      }
      else
      {
        char ttemp[8] = {0};
        int la, lb, ls;

        l++;
        r = get_st(now_task.message.c_str() + l, "~") + l;
        if (r == -1)
        {
          throw invalid_argument("invalid argument");
        }
        get_copy(l, r, now_task.message.c_str(), ttemp);
        l_a = ttemp;
        l = r + 1;
        r = get_st(now_task.message.c_str() + l, "#") + l;
        get_copy(l, r, now_task.message.c_str(), ttemp);
        l_b = ttemp;
        sscanf(l_a.c_str(), "%d", &la);
        sscanf(l_b.c_str(), "%d", &lb);
        sscanf(l_step.c_str(), "%d", &ls);

        if (la > 1920 || la <= 0 || lb > 1920 || lb <= 0 || la * lb * ls > 1920 * 1088 * 25 || (la & 63) || (lb & 63) || (ls < 1))
        {
          throw out_of_range("too large or cannot divided by 64");
        }
      }
      if ((!crt_method(l_method)))
      {
        throw out_of_range("invalid method");
      }
      {
        l = get_st(now_task.message.c_str(), "#");
        r = strlen(now_task.message.c_str());
        char str[1 << 12] = {0};
        get_copy(l + 1, r, now_task.message.c_str(), str);
        strchg("\\&#91;", "[", str);
        strchg("\\&#93;", "]", str);
        strchg("，", ",", str);
        strchg("（", "(", str);
        strchg("）", ")", str);
        strchg("\r\n", " ", str);
        strchg("\n", " ", str);
        string ss = str;
        now_task.message = "[OUTPUT_JOIN]";
        Basic_Message_Output(now_task, gener(l_a, l_b, l_step, ss, l_method));
      }
    }
    catch (std::exception &e)
    {
      Basic_Message_Output(now_task, e.what());
      tasklist.pop();
      continue;
    }
    tasklist.pop();
  }
  return 1;
}
#endif
