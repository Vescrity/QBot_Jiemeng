#ifndef URL_ORDERS
#define URL_ORDERS
#include <string>
#include "urls.h"
#include "Jiemeng_Basic.h"
using namespace std;
void Get_Para(const std::string &str, int n, std::vector<std::string> *result);
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
  }
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
  std::regex regex("\\{\\$(.*?)\\}");
  std::smatch match;
  std::string result = format;

  info_puts(to_string(js).c_str());
  while (std::regex_search(result, match, regex))
  {
    if (match.size() == 2)
    {
      std::string key = match[1].str();
      try
      {
        json::json_pointer ptr(key);
        if (!js.contains(ptr))
          throw std::invalid_argument("invalid json pointer " + key);
        std::string value = js[ptr].dump();
        result = str_strchg(("{$"s + key + "}").c_str(), value.substr(1, value.size() - 2).c_str(), result.c_str());
        // result = std::regex_replace(result, regex, value.substr(1, value.size() - 2));
      }
      catch (const std::exception &e)
      {
        std::string msg = "Exception caught: ";
        msg += e.what();
        error_lable("[url_ans_format]");
        error_puts(msg.c_str());
        break;
      }
    }
  }

  return result;
}
#endif