#ifndef TIME_STRUCT
#define TIME_STRUCT
#include <stdio.h>
#include <time.h>
struct str_tm
{
  time_t tself;
  char year[6];
  int nyear;
  char month[6];
  int nmonth;
  char mday[6];
  int nmday;
  char wday[6];
  int nwday;
  char yday[6];
  int nyday;
  char hour[3];
  int nhour;
  char min[3];
  int nmin;
  char sec[3];
  int nsec;
  str_tm()
  {
    time_t now = tself = time(0);
    tm *ltm = localtime(&now);
    nyear = 1900 + (ltm->tm_year);
    nmonth = 1 + (ltm->tm_mon);
    nmday = ltm->tm_mday;
    nhour=ltm->tm_hour;
    nmin = ltm->tm_min;
    nsec = ltm->tm_sec;
    sprintf(year, "%d", nyear);
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