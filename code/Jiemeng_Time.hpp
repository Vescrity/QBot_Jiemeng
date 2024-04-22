#ifndef JIEMENG_TIME
#define JIEMENG_TIME
#include <stdio.h>
#include <time.h>
#include <string>
#include <string.h>
using namespace std;
class Time_Class
{
public:
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
  string time_mark()
  {
    char str[10];
    sprintf(str, "[%02d:%02d]", nhour, nmin);
    return str;
  }
  string get_year() { return year; }
  string get_month() { return month; }
  string get_monthday() { return mday; }
  string get_weekday() { return wday; }
  string get_yearday() { return yday; }
  string get_hour() { return hour; }
  string get_min() { return min; }
  string get_sec() { return sec; }
  int nget_year() { return nyear; }
  int nget_month() { return nmonth; }
  int nget_monthday() { return nmday; }
  int nget_weekday() { return nwday; }
  int nget_yearday() { return nyday; }
  int nget_hour() { return nhour; }
  int nget_min() { return nmin; }
  int nget_sec() { return nsec; }
  Time_Class(time_t now = time(0))
  {
    tself = now;
    tm *ltm = localtime(&now);
    nyear = 1900 + (ltm->tm_year);
    nmonth = 1 + (ltm->tm_mon);
    nmday = ltm->tm_mday;
    nhour = ltm->tm_hour;
    nmin = ltm->tm_min;
    nsec = ltm->tm_sec;
    
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"

    sprintf(year, "%d", nyear);
    sprintf(month, "%d", 1 + (ltm->tm_mon));
    sprintf(mday, "%d", ltm->tm_mday);
    sprintf(hour, "%d", ltm->tm_hour);
    sprintf(min, "%d", ltm->tm_min);
    sprintf(sec, "%d", ltm->tm_sec);
    sprintf(wday, "%d", ltm->tm_wday);
    sprintf(yday, "%d", 1 + (ltm->tm_yday));

#pragma GCC diagnostic pop
  }
};
#endif