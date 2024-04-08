#ifndef JIEMENG_IO
#define JIEMENG_IO
#include <time.h>
#include <stdio.h>
#include <string>

using string = std::string;

extern bool Debug_Mode;

#include <chrono>
#include <iomanip>
inline void print_time_mark()
{
  auto now = std::chrono::system_clock::now();
  auto now_as_time_t = std::chrono::system_clock::to_time_t(now);
  auto now_tm = *std::localtime(&now_as_time_t);
  auto duration_since_epoch = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
  auto millis_since_epoch = millis.count();
  auto millis_remainder = millis_since_epoch % 1000;
  char time_str[100];
  strftime(time_str, sizeof(time_str), "%T", &now_tm);
  printf("\033[0m\033[0;37m[%s.%03d]", time_str, static_cast<int>(millis_remainder));
}
/*
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
*/
/// @brief CYAN
/// @param s
inline void msg_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[1;7;36m%s\033[0m", s);
}
inline void msg_print(const char *s)
{
  printf("\033[0m\033[1;36m%s\033[0m", s);
}
inline void msg_print(const string &s)
{
  printf("\033[0m\033[1;36m%s\033[0m", s.c_str());
}
inline void msg_puts(const char *s)
{
  printf("\033[0m\033[1;36m%s\n\033[0m", s);
}
inline void msg_puts(const string &s)
{
  printf("\033[0m\033[1;36m%s\n\033[0m", s.c_str());
}
/// @brief MAGENTA
/// @param s
inline void debug_print(const char *s)
{
  if (Debug_Mode)
    printf("\033[0m\033[35m%s\033[0m", s);
}
inline void debug_puts(const char *s)
{
  if (Debug_Mode)
    printf("\033[0m\033[35m%s\n\033[0m", s);
}
inline void debug_lable(const char *s)
{
  if (Debug_Mode)
  {
    print_time_mark();
    printf("\033[0m\033[7;35m%s\033[0m", s);
  }
}

inline void func_show(const char *s)
{
#ifdef _FUNC_SHOW_
  debug_puts(s);
#endif
}
/// @brief GREEN
/// @param s
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

inline void info_puts(const string &s)
{
  printf("\033[0m\033[1;32m%s\n\033[0m", s.c_str());
}

/// @brief BLUE
/// @param s
inline void color_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[1;7;34m%s\033[0m", s);
}
inline void color_puts(const char *s)
{
  printf("\033[0m\033[1;34m%s\n\033[0m", s);
}
inline void color_print(const string &s)
{
  printf("\033[0m\033[1;34m%s\033[0m", s.c_str());
}
/// @brief YELLOW
/// @param s
inline void warn_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[1;7;33m%s\033[0m", s);
}
inline void warn_puts(const char *s)
{
  printf("\033[0m\033[1;33m%s\n\033[0m", s);
}
inline void warn_print(const char *s)
{
  printf("\033[0m\033[1;33m%s\033[0m", s);
}
/// @brief RED
/// @param s
inline void error_lable(const char *s)
{
  print_time_mark();
  printf("\033[0m\033[5;7;31m%s\033[0m", s);
}
inline void error_puts(const string &s)
{
  printf("\033[0m\033[5;31m%s\n\033[0m", s.c_str());
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

#endif