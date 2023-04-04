#ifndef TXT2IMG_API
#define TXT2IMG_API
#include <string>
#include "urls.h"
using string = std::string;
string txt2img(const char *txt)
{
  const int N = 16;
  static int Now = 0;
  char *filename = new char[16];
  sprintf(filename, "text%03d.png", Now++);
  if (Now >= N)
    Now = 0;
  string Fname = filename;
  Fname = "tmp/" + Fname;
  delete[] filename;
  Request *req = new Request;
  req->set_url("http://api.setbug.com");
  req->set_api("/tools/text2image/");
  json dta;
  dta["break_width"] = 1080;
  dta["font_color"] = "00ffffff";
  dta["background_color"] = "00000000";
  dta["text"] = string(txt);
  req->set_data(dta);
  FILE *fp = fopen(Fname.c_str(), "wb");
  req->FGet(fp);
  fclose(fp);
  delete req;
  string rt;
#ifdef _WIN32
  rt = "[CQ:image,file=file:///./" + Fname + "]";
#else
  char RUN_PATH[50];
  memset(RUN_PATH, 0, sizeof RUN_PATH);
  execmd("pwd", RUN_PATH);
  rt = "[CQ:image,file=file:///";
  rt = rt + RUN_PATH + "/" + Fname + "]";
#endif

  return rt;
}
#endif