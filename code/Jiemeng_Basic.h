#ifndef JIEMENG_BASIC
#define JIEMENG_BASIC
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string>
using string = std::string;
inline void minisleep(const int &x)
{
#ifndef _WIN32
  usleep(x << 10);
#else
  Sleep(x);
#endif
}

inline string execmd(const string &commd)
{
  int bufsize = 1024;
  string output;
  char buf[bufsize];
  FILE *fp = popen(commd.c_str(), "r");
  if (fp == NULL)
  {
    return "";
  }
  while (fgets(buf, bufsize, fp))
  {
    output += buf;
  }
  pclose(fp);
#ifdef _WIN32
  if (hasNonUTF8(output))
  {
    string str = GBKToUTF8(output.c_str());
    return str;
  }
#endif
  return output;
}

#endif
