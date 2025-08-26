#include "Jiemeng_IO.hpp"
const short BLUE = -2;
const short MESSAGE = -1;
const short DEBUG = 0;
const short INFO = 1;
const short WARN = 2;
const short ERROR = 3;
const short LABLE = 1;
const short PRINT = 2;
const short PUTS = 3;

using string = std::string;

extern bool Debug_Mode;

#include <chrono>
void print_time_mark() {
    auto now = std::chrono::system_clock::now();
    auto now_as_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_as_time_t);
    auto duration_since_epoch = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
        duration_since_epoch);
    auto millis_since_epoch = millis.count();
    auto millis_remainder = millis_since_epoch % 1000;
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%T", &now_tm);
    printf("\033[0m\033[0;37m[%s.%03d]", time_str,
           static_cast<int>(millis_remainder));
}

void basic_IO(const string &s, short level, short type) {
    if (level == DEBUG && !Debug_Mode)
        return;
    string colors[] = {"34m", "36m", "35m", "32m", "33m", "31m"};
    if (type == LABLE)
        print_time_mark();
    printf("\033[0m\033[%s%s%s%s%s\033[0m", (level == DEBUG) ? "" : "1;",
           (type == LABLE) ? "7;" : "", colors[level + 2].c_str(), s.c_str(),
           (type == PUTS) ? "\n" : "");
}
void blue_lable(const char *s) { basic_IO(s, BLUE, LABLE); }
void blue_print(const string &s) { basic_IO(s, BLUE, PRINT); }
void blue_puts(const string &s) { basic_IO(s, BLUE, PUTS); }
void msg_lable(const char *s) { basic_IO(s, MESSAGE, LABLE); }
void msg_print(const string &s) { basic_IO(s, MESSAGE, PRINT); }
void msg_puts(const string &s) { basic_IO(s, MESSAGE, PUTS); }
void debug_lable(const char *s) { basic_IO(s, DEBUG, LABLE); }
void debug_print(const string &s) { basic_IO(s, DEBUG, PRINT); }
void debug_puts(const string &s) { basic_IO(s, DEBUG, PUTS); }
void info_lable(const char *s) { basic_IO(s, INFO, LABLE); }
void info_print(const string &s) { basic_IO(s, INFO, PRINT); }
void info_puts(const string &s) { basic_IO(s, INFO, PUTS); }
void warn_lable(const char *s) { basic_IO(s, WARN, LABLE); }
void warn_print(const string &s) { basic_IO(s, WARN, PRINT); }
void warn_puts(const string &s) { basic_IO(s, WARN, PUTS); }
void error_lable(const char *s) { basic_IO(s, ERROR, LABLE); }
void _error_print(const string &s) { basic_IO(s, ERROR, PRINT); }
void _error_puts(const string &s) { basic_IO(s, ERROR, PUTS); }
inline void exception_show(const string &lable, const string &msg) {
    error_lable(lable.c_str());
    error_puts(msg);
}
