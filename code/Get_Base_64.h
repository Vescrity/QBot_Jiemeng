#ifndef GET_BASE_64
#define GET_BASE_64
#include "urls.h"

#include <iostream>
#include <fstream>
#include <string>
#include <boost/beast/core/detail/base64.hpp>

static std::string file_to_base64(const std::string &filename)
{
  std::ifstream ifs(filename, std::ios::binary);
  if (!ifs)
  {
    throw std::runtime_error("Failed to open file");
  }
  std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  std::size_t base64_len = boost::beast::detail::base64::encoded_size(data.size());
  // 分配缓冲区
  std::string base64_str(base64_len, '\0');
  // 对文件内容进行 Base64 编码
  std::size_t result_len = boost::beast::detail::base64::encode(&base64_str[0], data.data(), data.size());
  // 确保编码结果长度正确
  if (result_len != base64_len)
  {
    throw std::runtime_error("Base64 encoding failed");
  }

  // 返回编码结果
  return base64_str;
}
static string get_img_url(const string &msg)
{
  auto st = msg.find("url=");
  if (st == string::npos)
    throw std::runtime_error("No url found");
  auto ed = msg.substr(st).find("]");
  ed += st;
  st += 4;
  return msg.substr(st, ed);
}
static string get_base64_from_url(const string &url)
{
  Request *rq = new Request;
  rq->set_url(url);
  const int N = 16;
  static int Now = 0;
  char *filename = new char[16];
  sprintf(filename, "tmp/from%03d.png", Now++);
  string Fname=filename;
  delete[] filename;
  if (Now >= N)
    Now = 0;
  FILE *fp = fopen(Fname.c_str(), "wb");
  rq->FGet(fp);
  fclose(fp);
  delete rq;
  return file_to_base64(Fname);
}
string get_base64_from_message(const string &msg){
  return get_base64_from_url(get_img_url(msg));
}

#endif