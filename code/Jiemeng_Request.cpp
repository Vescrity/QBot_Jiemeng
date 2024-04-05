#include "Jiemeng_Request.hpp"
#include "Jiemeng_LogIO.hpp"
#include <sstream>
#include "opstring.h"
#include "Jiemeng_Basic.h"
#include <curl/curl.h>
json Request::js_curl_get()
{
  string ss = curl_get();
  try
  {
    json sjs = json::parse(ss);
    return sjs;
  }
  catch (...)
  {
    return json::parse("null");
  }
}
json Request::js_curl_post()
{
  string ss = curl_post();
  try
  {
    json sjs = json::parse(ss);
    return sjs;
  }
  catch (...)
  {
    return json::parse("null");
  }
}
json Request::js_get()
{
  string ss = Get();
  try
  {
    json sjs = json::parse(ss);
    return sjs;
  }
  catch (...)
  {
    return json::parse("null");
  }
}
json Request::js_post()
  {
    string ss = Post();
    try
    {
      json sjs = json::parse(ss);
      return sjs;
    }
    catch (const std::exception &e)
    {
      std::string msg = "Exception caught: ";
      msg += e.what();
      error_lable("[Post]");
      error_puts(msg.c_str());
      error_lable("[Post]");
      error_puts(ss.c_str());
      return json::parse("null");
    }
  }
string Request::datas_urlencode()
{
  string rt = "";
  int i = 0;
  for (auto &elem : data.items())
  {
    rt = rt + ((i == 0) ? "?" : "&");
    string tmp = string(elem.key());
    rt = rt + url_encode(tmp) + "=";
    if (elem.value().is_number())
      tmp = num2str(elem.value());
    else
      tmp = string(elem.value());
    rt = rt + url_encode(tmp);
    i++;
  }
  return rt;
}
string Request::curl_get()
{
  string cmds = "curl -s \"";
  cmds = cmds + url_link() + datas_urlencode();
  char *temps = new char[1 << 14];
  memset(temps, 0, sizeof temps);
  cmds = cmds + "\"";
  return execmd(cmds);
}
//?
static size_t Wset(void *buf, size_t size, size_t nm, void *lp)
{
  string *str = dynamic_cast<string *>((string *)lp);
  if (str == NULL || buf == NULL)
    return -1;
  char *pd = (char *)buf;
  str->append(pd, size * nm);
  return nm;
}
// libcurl-Post
string Request::Post()
{
  string rt;
  if (js_default)
  {
    add_Headers("Content-Type: application/json");
    msgs = to_string(data);
  }
  CURL *curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("CURL_INIT_ERROR");
  int nhds = Headers.size();
  if (nhds)
  {
    curl_slist *headers = NULL;
    for (int i = 0; i < nhds; i++)
    {
      headers = curl_slist_append(headers, Headers[i].c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  }

  curl_easy_setopt(curl, CURLOPT_URL, url_link().c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msgs.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Wset);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&rt);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return rt;
}
FILE *Request::FPost(FILE *fp)
{
  if (js_default)
  {
    add_Headers("Content-Type: application/json");
    msgs = to_string(data);
  }
  CURL *curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("CURL_INIT_ERROR");
  int nhds = Headers.size();
  if (nhds)
  {
    curl_slist *headers = NULL;
    for (int i = 0; i < nhds; i++)
    {
      headers = curl_slist_append(headers, Headers[i].c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  }

  curl_easy_setopt(curl, CURLOPT_URL, url_link().c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msgs.c_str());

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return fp;
}
string Request::Get()
{
  string rt;
  CURL *curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("CURL_INIT_ERROR");
  curl_easy_setopt(curl, CURLOPT_URL, (url_link() + datas_urlencode()).c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Wset);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&rt);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return rt;
}
FILE *Request::FGet(FILE *fp)
{
  CURL *curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("CURL_INIT_ERROR");
  curl_easy_setopt(curl, CURLOPT_URL, (url_link() + datas_urlencode()).c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return fp;
}
string Request::curl_post()
{
  if (js_default)
  {
    add_Headers("Content-Type: application/json");
  }
  string p;
  p = js_default ? to_string(data) : msgs;
  string len = num2str(p.length());
  msgs = escape_string(p);

  add_Headers("Content-Length: " + len);
  string cmds = "curl -s -X POST ";
  int nsize = Headers.size();
  for (int i = 0; i < nsize; i++)
    cmds = cmds + "-H \"" + Headers[i] + "\" ";
  cmds = cmds + "-d " + msgs + " \"" + url_link() + "\"";
  return execmd(cmds);
}
