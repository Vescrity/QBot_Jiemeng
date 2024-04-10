#ifndef JIEMENG_IO
#define JIEMENG_IO
#include <time.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <iomanip>
using string = std::string;

void print_time_mark();
/// @brief CYAN
void msg_lable(const char *);
void msg_print(const string &);
void msg_puts(const string &);
/// @brief MAGENTA
void debug_lable(const char *);
void debug_print(const string &);
void debug_puts(const string &);

void func_show(const char *);
/// @brief GREEN
void info_lable(const char *);
void info_print(const string &);

void info_puts(const string &);
/// @brief BLUE
void blue_lable(const char *);
void blue_puts(const string &);
void blue_print(const string &s);
/// @brief YELLOW
void warn_lable(const char *s);
void warn_puts(const string &s);
void warn_print(const string &s);
/// @brief RED
/// @param s
void error_lable(const char *s);
void error_puts(const string &s);
void error_print(const string &);
void exception_show(const string &lable, const string &msg);

#endif