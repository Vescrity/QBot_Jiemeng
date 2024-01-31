#ifndef TIME_STRUCT
#define TIME_STRUCT
#include <stdio.h>
#include <time.h>
struct str_tm
{
  char year[6];
  char month[6];
  char mday[6];
  char wday[6];
  char yday[6];
  char hour[3];
  char min[3];
  char sec[3];
  str_tm()
  {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    sprintf(year, "%d", 1900 + (ltm->tm_year));
    sprintf(month, "%d", 1 + (ltm->tm_mon));
    sprintf(mday, "%d", ltm->tm_mday);
    sprintf(hour, "%d", ltm->tm_hour);
    sprintf(min, "%d", ltm->tm_min);
    sprintf(sec, "%d", ltm->tm_sec);
    sprintf(wday, "%d", ltm->tm_wday);
    sprintf(yday, "%d", 1 + (ltm->tm_yday));
  }
};
#endif