#ifndef JIEMENG_RANDOM
#define JIEMENG_RANDOM
#include <stdio.h>
#include <random>
#include <chrono>
#include <time.h>
#include <string.h>
#include <boost/regex.hpp>
#include "opstring.h"

#define Lint long long
Lint Rands(Lint l, Lint r);
Lint adb(Lint a, Lint b);

std::string chg_Rcode(const std::string &Rstr);
#endif
