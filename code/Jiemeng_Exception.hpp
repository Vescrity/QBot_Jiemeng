#ifndef JM_EXCEPTION

#include <string>
#include "Jiemeng_LogIO.hpp"

/// @brief 通用异常打印宏
/// @param LABLE 标签/类别
#define JM_EXCEPTION(LABLE) \
std::string msg = "Exception caught: ";\
msg += e.what();\
error_lable((LABLE));\
error_puts(msg.c_str());
#endif