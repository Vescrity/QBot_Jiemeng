#ifndef JIEMENG_BASIC
#define JIEMENG_BASIC

#define JIEMENG_VERSION ("v25.0")
#ifdef _WIN32
#define JIEMENG_PLATFORM "Jiemeng_Windows (Limited Feature)"
#else
#define JIEMENG_PLATFORM "Jiemeng_Linux"
#endif
#define UPDATE_TIME __DATE__ + " " + __TIME__


#include <string.h>
#include <stdio.h>
#include "opstring.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <iostream>



inline void minisleep(const int x)
{
#ifndef _WIN32
  usleep(x * 1000);
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
