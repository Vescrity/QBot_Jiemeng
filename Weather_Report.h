#ifndef WEATHER_REPORT
#define WEATHER_REPORT
#include <string>
#include "Jiemeng_Basic.h"
using namespace std;
struct _1day_report
{
  string cond_day, cond_night, date, hum,
      sun_rise, moon_rise, sun_set, moon_set,
      max_tmp, min_tmp, pop, pcpn, cloud,
      wind_dir, wind_spd, wind_sc;
  string _7dprint()
  {
    string rt;
    rt = rt + date + "\n" + "日间：" + cond_day + " " + max_tmp + "℃" + "\n";
    rt = rt + "夜间：" + cond_night + " " + min_tmp + "℃" + "\n";
    rt = rt + "湿度：" + hum + "%\n";
    rt = rt + "降水概率：" + pop + "%\n降水量：" + pcpn + "\n";
    rt = rt + wind_dir + wind_sc + "级，" + wind_spd + "kph\n";
    rt = rt + "日出：" + sun_rise + " 日落：" + sun_set + "\n";
    return rt;
  }
  string _24hprint()
  {
    string rt;
    rt = rt + date + "\n" + cond_day + " " + max_tmp + "℃" + "  ";
    rt = rt + "湿度：" + hum + "%\n";
    rt = rt + "降水概率：" + pop + "%\n";
    rt = rt + wind_dir + wind_sc + "级，" + wind_spd + "kph\n";
    return rt;
  }
  void _7dinit(const char *msg)
  {
    cond_day = json_read(msg, "cond_txt_d");
    cond_night = json_read(msg, "cond_txt_n");
    wind_dir = json_read(msg, "wind_dir");
    wind_spd = json_read(msg, "wind_spd");
    max_tmp = json_read(msg, "tmp_max");
    min_tmp = json_read(msg, "tmp_min");
    wind_sc = json_read(msg, "wind_sc");
    date = json_read(msg, "date");
    pcpn = json_read(msg, "pcpn");
    pop = json_read(msg, "pop");
    hum = json_read(msg, "hum");
    moon_rise = json_read(msg, "mr");
    moon_set = json_read(msg, "ms");
    sun_rise = json_read(msg, "sr");
    sun_set = json_read(msg, "ss");
  }
  void _24hinit(const char *msg)
  {
    cond_day = json_read(msg, "cond_txt");
    date = json_read(msg, "time");
    hum = json_read(msg, "hum");
    max_tmp = json_read(msg, "tmp");
    pop = json_read(msg, "pop");
    wind_dir = json_read(msg, "wind_dir");
    wind_spd = json_read(msg, "wind_spd");
    wind_sc = json_read(msg, "wind_sc");
  }
};
struct City
{
  string locat, parent, admin_area, cnty, lat, lon;
  _1day_report report[20];
  int day_cnt;
  bool ok;
  string basic_print()
  {
    if (!ok)
      return "ERROR";
    string rt;
    rt = rt + cnty + " " + admin_area + " " + parent + " " + locat + "\n纬度：" + lat + "\n经度：" + lon + "\n";
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
    sprintf(para, "plot.exe ");
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
      sprintf(para + strlen(para), "%s,", report[i].pop.c_str());
    }
    system(para);
  }
  void get_24hplot()
  {
    char para[1 << 10] = {0};
    sprintf(para, "plot.exe ");
    sprintf(para + strlen(para), "%d,24,", day_cnt);
    for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", report[i].max_tmp.c_str());
    }
    for (int i = 0; i < day_cnt; i++)
    {
      sprintf(para + strlen(para), "%s,", report[i].pop.c_str());
    }
    system(para);
  }
  string outplot()
  {
    char file_path[1 << 9];
    memset(file_path, 0, sizeof file_path);
    getcwd(file_path, sizeof file_path);
    strchg("\\", "/", file_path);
    string rt = rt + "[CQ:image,file=file:///" + file_path + "/qwq.jpg]";
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
  void city_basic_init(const char *msg)
  {
    int l, r;
    l = get_st(msg, "basic") + 7;
    r = get_st(msg, "},") + 1;
    char ss[1 << 15] = {0};
    get_copy(l, r, msg, ss);
    locat = json_read(ss, "location");
    parent = json_read(ss, "parent_city");
    admin_area = json_read(ss, "admin_area");
    cnty = json_read(ss, "cnty");
    lat = json_read(ss, "lat");
    lon = json_read(ss, "lon");
  }
  void city_7d_init(const char *msg)
  {
    string msgg;
    msgg = json_read(msg, "msg");
    if (msgg != "success")
    {
      ok = 0;
      return;
    }
    ok = 1;
    int l, r;
    city_basic_init(msg);
    char ss[1 << 15] = {0};
    day_cnt = 7;
    r = get_st(msg, "status");
    r = get_st(msg + r, "[{") + r + 1;
    for (int i = 0; i < day_cnt; i++)
    {
      l = r;
      r = get_st(msg + l, "}") + 1 + l;
      get_copy(l, r, msg, ss);
      report[i]._7dinit(ss);
    }
  }
  void city_24h_init(const char *msg)
  {
    string msgg;
    msgg = json_read(msg, "msg");
    if (msgg != "success")
    {
      ok = 0;
      return;
    }
    ok = 1;
    int l, r;
    city_basic_init(msg);
    char ss[1 << 15] = {0};
    day_cnt = 8;
    r = get_st(msg, "status");
    r = get_st(msg + r, "[{") + r + 1;
    for (int i = 0; i < day_cnt; i++)
    {
      l = r;
      r = get_st(msg + l, "}") + 1 + l;
      get_copy(l, r, msg, ss);
      report[i]._24hinit(ss);
    }
  }
};

#endif
