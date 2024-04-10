#ifndef JIEMENG_RANDOM
#define JIEMENG_RANDOM
#include <stdio.h>
#include <random>
#include <chrono>
#include <time.h>
#include <string.h>
#include <boost/regex.hpp>

#define Lint long long
/// @brief 返回[l,r]区间内随机整数
Lint Rands(Lint l, Lint r);
/// @brief 投掷 a 次 b 面骰子的和
Lint adb(Lint a, Lint b);

std::string chg_Rcode(const std::string &Rstr);
#endif
