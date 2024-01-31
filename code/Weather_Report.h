#ifndef WEATHER_REPORT
#define WEATHER_REPORT
#include <string>
#include "Jiemeng_Basic.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;
struct _1day_report
{
  string cond_day, cond_night, date, hum,
      sun_rise, moon_rise, sun_set, precipitation,
      max_tmp, min_tmp, cloud, air, air_level,
      wind_sc, wind_day, wind_day_levle, wind_night, wind_night_level;
  string _7dprint()
  {
    string rt;
    rt = rt + date + "\n" + "日间：" + cond_day + " " + max_tmp + "℃ " + wind_day + wind_day_levle + "\n";
    rt = rt + "夜间：" + cond_night + " " + min_tmp + "℃" + wind_night + wind_night_level + "\n";
    rt = rt + "空气: " + air + air_level + "\n";
    rt = rt + "降水量：" + precipitation + "\n";
    rt = rt + "日出：" + sun_rise + " 日落：" + sun_set + "\n";
    return rt;
  }
  string _24hprint()
  {
    string rt;
    rt = rt + date + "\n" + cond_day + " " + max_tmp + "℃" + "  ";
    return rt;
  }
  void _7dinit(const json &js)
  {
    cond_day = js["wea_day"];
    cond_night = js["wea_night"];
    wind_day = js["wind_day"];
    wind_night = js["wind_night"];
    wind_day_levle = js["wind_day_level"];
    wind_night_level = js["wind_night_level"];
    air = js["air"];
    air_level = js["air_level"];
    max_tmp = js["temp_day"];
    min_tmp = js["temp_night"];
    date = js["date"];
    precipitation = js["precipitation"];
    sun_rise = js["sunrise"];
    sun_set = js["sunset"];
  }
  void _24hinit(const json &js)
  {
  }
};
struct City
{
  string locat, parent, admin_area, lat, lon;
  _1day_report report[20];
  int day_cnt;
  bool ok;
  string basic_print()
  {
    if (!ok)
      return "ERROR";
    string rt;
    rt = rt + " " + admin_area + " " + parent + " " + locat + "\n";
    return rt;
  }
  string _7dprint()
  {
    if (!ok)
      return "ERROR";
    string rt;
    rt = basic_print();
    for (int i = 0; i < day_cnt; i++)
    {
      rt = rt + report[i]._7dprint() + "\n";
    }

    return rt;
  }

  void get_7dplot()
  {
    char para[1 << 10] = {0};
#ifdef MATLABEXE
    sprintf(para, "plot.exe ");
#else
    sprintf(para, R"(cd jm_script; octave --no-gui --eval 'pkg load image; weather_plot(")");
#endif

    sprintf(para + strlen(para), "%d,7,", day_cnt);
    for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", report[i].max_tmp.c_str());
    }
    for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", report[i].min_tmp.c_str());
    }
    for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", "0");
    }
#ifndef MATLABEXE
    sprintf(para + strlen(para), R"(")')");
#endif

    system(para);
  }
  void get_24hplot()
  {
    char para[1 << 10] = {0};
#ifdef MATLABEXE
    sprintf(para, "plot.exe ");
#else
    sprintf(para, R"(cd jm_script; octave --no-gui --eval 'pkg load image; weather_plot(")");
#endif
    sprintf(para + strlen(para), "%d,24,", day_cnt);
    for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", report[i].max_tmp.c_str());
    }
    /*for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", report[i].pop.c_str());
    }*/
#ifndef MATLABEXE
    sprintf(para, R"(")')");
#endif
    system(para);
  }
  string outplot()
  {
    char file_path[1 << 9];
    memset(file_path, 0, sizeof file_path);
    getcwd(file_path, sizeof file_path);
#ifdef _WIN32
    strchg("\\", "/", file_path);
#endif
    string rt = rt + "[CQ:image,file=file:///" + file_path + "/tmp/qwq.png]";
    return rt;
  }
  string _24hprint()
  {
    if (!ok)
      return "ERROR";
    string rt;
    rt = basic_print();
    for (int i = 0; i < day_cnt; i++)
    {
      rt = rt + report[i]._24hprint() + "\n";
    }
    return rt;
  }
  void city_basic_init(const json &js)
  {
    int l, r;

    locat = js["city"];
    parent = js["leader"];
    admin_area = js["province"];

    // lat = json_read(ss, "lat");
    // lon = json_read(ss, "lon");
  }

  void city_7d_init(const json &js)
  {
    string msgg;

    msgg = js["msg"];
    if (msgg != "success")
    {
      ok = 0;
      return;
    }
    ok = 1;
    int l, r;
    city_basic_init(js["data"][0]);
    char ss[1 << 15] = {0};
    day_cnt = 7;

    for (int i = 0; i < day_cnt; i++)
    {
      report[i]._7dinit(js["data"][i]);
    }
  }
  void city_24h_init(const json &js)
  {
    string msgg;

    msgg = js["msg"];
    if (msgg != "success")
    {
      ok = 0;
      return;
    }
    ok = 1;
    int l, r;
    city_basic_init(js["data"][0]);
    char ss[1 << 15] = {0};
    day_cnt = 7;

    for (int i = 0; i < day_cnt; i++)
    {
      report[i]._24hinit(js["data"][i]);
    }
  }
};

#endif
