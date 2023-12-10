/***************************

  * plugin模版v0.1

  **  注意事项：所有通过stdout流输出的内容都会作为输出消息

***************************/
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
const std::string PLUGIN_NAME = "mota";                      // 插件名称
const std::string PLUGIN_PATH = ".\\plugin\\" + PLUGIN_NAME; // 插件路径，运行中产生的文件请保存到这个目录下
std::string plugin_function(std::string);
int main(int a, char **ss)
{
#ifdef _WIN32
  WinExec("chcp 65001", SW_HIDE);
#endif
  FILE *fp;
  fp = fopen("Current_msg.txt", "r");
  int c;
  char ms[1 << 13] = {0};
  int i = 0;
  while ((c = fgetc(fp)) != EOF)
  {
    ms[i++] = c;
  }
  printf("%s", plugin_function(std::string(ms)).c_str());
  return 0;
}
#include "../opstring.h"
#include "../Jiemeng_Basic.h"
#include "../urls.h"

using namespace std;

std::string plugin_function(std::string msgs)
{
  int count = 10;
  msgs = decode(msgs);
  int sl = get_st(msgs.c_str(), "#");
  int sr = get_st(msgs.c_str(), "~");
  if (sr == -1)
    return "ERROR";
  int r2 = get_st(msgs.c_str() + sr + 1, "~");
  if (r2 == -1)
    return "ERROR";
  r2 += sr + 1;
  char *tmp = new char[msgs.length()];
  string name, dif, ending;
  get_copy(sl + 1, sr, msgs.c_str(), tmp);
  name = tmp;
  get_copy(sr + 1, r2, msgs.c_str(), tmp);
  dif = tmp;
  get_copy(r2 + 1, msgs.length(), msgs.c_str(), tmp);
  ending = tmp;

  json data;
  data["name"] = name;
  data["difficulty"] = dif;
  data["ending"] = ending;
  data["count"] = count;
  data["verifiedOnly"] = "true";

  Request *req = new Request;
  req->set_url("https://h5mota.com/backend/tower");
  req->set_api("/fetchMoreSubmissions.php");
  req->set_data(data);
  json result = (req->js_curl_get());
  result = result["submissions"];

  delete req;
  if (result.empty())
    return "No Data";
  string rt;
  count = result.size();
  for (int i = 0; i < count; i++)
  {
    json &jtmp = result[i];
    rt = rt + "Rank " + num2str(i + 1);
    rt = rt + "\n用户：" + string(jtmp["user"]["name"]) + "(" + num2str(jtmp["user"]["id"]) + ")\n";
    rt = rt + "分数：" + string(jtmp["score"]) + "\n";
  }
  return rt;
}
