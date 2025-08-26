#ifndef JM_EXCEPTION

#include "Jiemeng_LogIO.hpp"
#include <string>

/// @brief 通用异常打印宏
/// @param LABLE 标签/类别
#define JM_EXCEPTION(LABLE)                                                    \
    std::string msg = "Exception caught: ";                                    \
    msg += "\nin file " __FILE__ ""s + " line:" + std::to_string(__LINE__);    \
    msg += "\nin function "s + __FUNCTION__;                                   \
    msg += "\n"s;                                                              \
    msg += e.what();                                                           \
    error_lable((LABLE));                                                      \
    _error_puts(msg);

class Not_Serious : public std::exception {};

#endif
