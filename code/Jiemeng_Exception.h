#ifndef JM_EXCEPTION
#include <string>
#include "Jiemeng_Basic.h"
#define JM_EXCEPTION(LABLE) \
std::string msg = "Exception caught: ";\
msg += e.what();\
error_lable((LABLE));\
error_puts(msg.c_str());
#endif