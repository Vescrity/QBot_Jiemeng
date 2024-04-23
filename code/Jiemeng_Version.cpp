#include "Jiemeng_Version.hpp"
#ifdef _WIN32
const string JIEMENG_PLATFORM = "Jiemeng_Windows";
#else
const string JIEMENG_PLATFORM = "Jiemeng_Linux";
#endif
const string UPDATE_TIME = string(__DATE__) + " " + __TIME__;
const string JIEMENG_VERSION = "v25.0.70";
