#ifndef URL_ORDERS
#define URL_ORDERS
#include <string>
#include "urls.h"
#include "Jiemeng_Basic.h"
#include "Jiemeng_Algorithm.h"
using namespace std;

void replaceStringValue(json &data, const json &para, int para_num)
{
  for (json::iterator it = data.begin(); it != data.end(); ++it)
  {
    if (it.value().type_name() == "string")
    {
      string str = it.value();
      if (str.find("##") == 0)
      {
        int n;
        sscanf(str.c_str(), "##%d", &n);
        if (n > para_num || n < 1)
          throw std::out_of_range("an invalid n is given at run_api order");
        it.value() = para[n - 1];
      }
    }
    else if (it.value().is_object() || it.value().is_array())
    {
      replaceStringValue(it.value(), para, para_num); // 递归调用以处理嵌套的JSON对象
    }
  }
}

std::string url_ans_format(const json &js, const std::string &format);
std::string url_order(const string &msg, const string &order, const json &js)
{

  json data = js["data"];
  Request *rq = new Request;
  rq->set_api(js["api"]);
  rq->set_url(js["url"]);
  int para_num = 0;
  vector<string> para;
  if (js.count("para_num"))
  {
    para_num = js["para_num"];
    if (msg.find("#") != string::npos)
      Get_Para(msg.substr(msg.find("#") + 1), para_num, &para);
    else
      throw std::invalid_argument("the input string does not contain expected character '#'");
  }
  replaceStringValue(data,para,para_num);
  rq->set_data(data);
  if (js.count("Get"))
    data = rq->js_get();
  else
    data = rq->js_post();
  delete rq;
  if (js.count("form"))
    return url_ans_format(data, js["form"]);
  else
    throw std::invalid_argument("the json does not contain expected key 'form'");
}
std::string url_ans_format(const json &js, const std::string &format)
{
  return string_format_with_json("{$", "}", js, format);
}
#endif