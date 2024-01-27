#ifndef JIEMENG_BASIC
#define JIEMENG_BASIC
#ifndef _WIN32
#define LINUX_V
#endif

#define JIEMENG_VERSION ("v23.0.0")
#ifdef _WIN32
#define JIEMENG_PLATFORM "Jiemeng"
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
#include "r_adb.h"
#include "Jiemeng_Exception.h"
inline void minisleep(const int x)
{
#ifndef _WIN32
  usleep(x * 1000);
#else
  Sleep(x);
#endif
}
inline void print_time_mark()
{
  int hr, mn, sec;
  time_t tmpcal_ptr;
  struct tm *tmp_ptr = NULL;
  time(&tmpcal_ptr);
  tmp_ptr = localtime(&tmpcal_ptr);
  hr = tmp_ptr->tm_hour;
  mn = tmp_ptr->tm_min;
  sec = tmp_ptr->tm_sec;
  printf("\033[0m\033[0;37m[%02d:%02d:%02d]", hr, mn, sec);
}
inline void msg_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[1;7;36m%s\033[0m", s);
}
inline void msg_print(const char *s)
{
  printf("\033[0m\033[1;36m%s\033[0m", s);
}
inline void msg_puts(const char *s)
{
  printf("\033[0m\033[1;36m%s\n\033[0m", s);
}
inline void debug_print(const char *s)
{
#ifdef _DEBUG_MODE_
  printf("\033[0m\033[7;32m%s\033[0m", s);
#endif
}
inline void debug_puts(const char *s)
{
#ifdef _DEBUG_MODE_
  printf("\033[0m\033[7;32m%s\n\033[0m", s);
#endif
}
inline void func_show(const char *s)
{
#ifdef _FUNC_SHOW_
  debug_puts(s);
#endif
}
inline void info_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[1;7;32m%s\033[0m", s);
}
inline void info_print(const char *s)
{
  printf("\033[0m\033[1;32m%s\033[0m", s);
}
inline void info_puts(const char *s)
{
  printf("\033[0m\033[1;32m%s\n\033[0m", s);
}
inline void color_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[1;7;34m%s\033[0m", s);
}
inline void color_puts(const char *s)
{
  printf("\033[0m\033[1;34m%s\n\033[0m", s);
}
inline void color_print(const char *s)
{
  printf("\033[0m\033[1;34m%s\033[0m", s);
}
inline void error_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[5;7;31m%s\033[0m", s);
}
inline void error_puts(const char *s)
{
  printf("\033[0m\033[5;31m%s\n\033[0m", s);
}
inline void error_print(const char *s)
{
  printf("\033[0m\033[5;31m%s\033[0m", s);
}
inline void exception_show(const string &lable, const string &msg)
{
  error_lable(lable.c_str());
  error_puts(msg.c_str());
}
string execmd(const string &commd)
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

#define MAX_MAIN_THR_NUM 512
thread Main_thread_list[MAX_MAIN_THR_NUM];
struct Thread_num
{
  int num;
  int get_num() { return num; }
  int get_new_num()
  {
    num++;
    if (num >= MAX_MAIN_THR_NUM)
      num = 0;
    return num;
  }
} Main_thr_num;

#endif
