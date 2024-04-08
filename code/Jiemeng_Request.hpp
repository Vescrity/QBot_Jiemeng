#ifndef JIEMENG_REQUEST
#define JIEMENG_REQUEST

#include <nlohmann/json.hpp>
#include <vector>
using namespace std;
using json = nlohmann::json;

class Request
{
private:
  string urls;
  string api;
  string msgs;
  json data;
  bool is_default;
  bool js_default;
  vector<string> Headers;
  // JSON数据URL编码
  string datas_urlencode();
  // 合并url与api接口
  string url_link()
  {
    return urls + api;
  }

public:
  Request() { js_default = is_default = 1; }
  void set_url(const string &url) { urls = url; }
  void set_api(const string &str) { api = str; }
  void set_data(const json &dts) { data = dts; }
  void set_msgs(const string &dts)
  {
    js_default = 0;
    msgs = dts;
  }
  void add_Headers(const string &hds)
  {
    is_default = 0;
    Headers.push_back(hds);
  }
  string Post();
  FILE *FPost(FILE *);
  string Get();
  string curl_get();
  FILE *FGet(FILE *);
  string curl_post();
  json js_curl_get();
  json js_curl_post();
  json js_get();
  json js_post();
};
#endif