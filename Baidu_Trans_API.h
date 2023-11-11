#ifndef BAIDU_TRANS_API
#define BAIDU_TRANS_API
#include "md5.h"
#include <string>
#include "urls.h"

using string = std::string;
/// @brief A class to use the baidu translation api.
class Baidu_Trans_API
{
private:
  Request rq;
  string app_id, app_secret, salt;

public:
  Baidu_Trans_API()
  {
    rq.set_url("https://fanyi-api.baidu.com/api");
    rq.set_api("/trans/vip/translate");
    salt = "Salt_Jiemeng";
  }
  void set(const string &id, const string &sec)
  {
    app_id = id;
    app_secret = sec;
  }
  string operate(const string &order)
  {
    if (!app_id.length())
      throw runtime_error("app_id or app_secret unset");
    string_view opt_lang;
    string_view text;
    size_t l = order.find("#");
    if (l == string::npos)
    {
      opt_lang = "en";
      text = order;
    }
    else
    {
      opt_lang = order.substr(0, l);
      text = order.substr(l + 1);
    }

    json js;

    js["q"] = text;
    js["from"] = "auto";
    js["to"] = opt_lang;
    js["appid"] = app_id;
    js["salt"] = salt;
    js["sign"] = md5(app_id + string(text) + salt + app_secret);
    rq.set_data(js);
    string rt = rq.Get();
    try
    {
      return json::parse(rt)["trans_result"][0]["dst"];
    }
    catch (const exception &e)
    {
      return string(e.what()) + "\nserver info: "s + rt;
    }
  }
} baidu_trans;

#endif
