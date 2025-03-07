#include "Jiemeng_Time.hpp"
#include <string>

using namespace std;
Time_Class start_time;
string start_up_time()
{
  char times[100];
  sprintf(times, "%04d-%02d-%02d %02d:%02d:%02d",
          start_time.nyear, start_time.nmonth,
          start_time.nmday, start_time.nhour,
          start_time.nmin, start_time.nsec);
  return times;
}
