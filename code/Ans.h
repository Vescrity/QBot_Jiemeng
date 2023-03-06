#ifndef ANS
#define ANS
#include "json.hpp"
#include "Jiemeng_Basic.h"
#include "Msg_type.h"
#ifdef JIE_MENG_BASE
#include "Spj_Ans.h"
bool no_str_spj(Msg_type type, const char *msg);
#endif
#include <regex>
using namespace std;
using json = nlohmann::json;
bool check_priv(const char *);
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>

class Reg_thr
{
public:
  std::mutex mutexWait; // 互斥锁
  std::condition_variable condWait;
};
class Ans_grp
{
private:
  string reg_str;
  json jreg;
  vector<string> anss;
  vector<string> grps;
  vector<string> user;
  vector<int> lvs;
  bool ifpriv;
  int cnt;
  int pri;

  bool type_check(const Msg_type &type);

  int r_opt()
  {
    int r = Rands(0, lvs[cnt] - 1);
    for (int i = 1; i <= cnt; i++)
    {
      if (lvs[i] > r)
        return i - 1;
    }
    return cnt - 1;
  }

public:
  friend void ans_update();
  json fexport();
  void disp();
  bool operator<(const Ans_grp &a) { return pri > a.pri; }
  void get_init(const json &js);
  bool check(const Msg_type &type, const char *msg);
  string opt()
  {
    return anss[r_opt()];
  }
};
vector<Ans_grp> ans_gp;

#ifdef JIE_MENG_BASE
string status_msgs()
{
  char sttmtxt[200];
  memset(sttmtxt, 0, sizeof sttmtxt);
  sprintf(sttmtxt, "自%s至今，统计信息如下：[\\n]"
                   "累计启动：%d次[\\n]累计响应消息：%d次[\\n]"
                   "应答组：%d组",
          STAT_START_TIME.c_str(), start_time, msg_send_time, ans_gp.size());
  return sttmtxt;
}
string output_msg_chg(const Msg_type &type, string content);
string get_output(const Msg_type &type, const char *msg, int x)
{
  func_show("get_output");
  if (x == 0)
  {
    if (no_str_spj(type, msg))
      return "";
    string spjstr = str_spj(type, msg);
    if (spjstr != "[SPJ_CONTINUE]")
      return spjstr;
  }
  int n = ans_gp.size();
  string _1line;
  _1line = str_strchg("\n", " ", msg);
  _1line = str_strchg("\r", " ", _1line.c_str());
  for (int i = x; i < n; i++)
  {
    if (ans_gp[i].check(type, _1line.c_str()))
    {
      // return ans_gp[i].opt();
      string str = ans_gp[i].opt();
      if (str == "[IGNORE]")
        continue;
      // str=str_strchg("[FAILED]","",str.c_str());
      while (get_st(str.c_str(), "[IGNORE]") != -1)
      {
        string nstr = get_output(type, msg, i + 1);
        str = str_strchg_1("[IGNORE]", nstr.c_str(), str.c_str());
      }
      str = str_strchg("[FAILED]", "", str.c_str());

      return output_msg_chg(type, str);
    }
  }
  return "[FAILED]";
}
void gets_output(const Msg_type &type, const char *msg, string *str, Reg_thr *rthr)
{
  *str = get_output(type, msg, 0);
  rthr->condWait.notify_one();
}
string get_output(const Msg_type &type, const char *msg, Reg_thr *rthr)
{
  try
  {
    string str;
    int thr = Main_thr_num.get_new_num();
    Main_thread_list[thr] = thread((gets_output), (type), (msg), (&str), rthr);
    thread &Regs = Main_thread_list[thr];
    std::unique_lock<std::mutex> lockWait(rthr->mutexWait);
    std::cv_status cvsts = rthr->condWait.wait_for(lockWait, std::chrono::milliseconds(Time_Limit));
    if (cvsts == std::cv_status::timeout)
    {
      Regs.join();
      error_lable("[Get_Output]");
      error_puts("Time out!!");
      return "[FAILED]";
    }
    else
    {
      Regs.join();
      return str;
    }
    // return 0;
  }
  catch (std::exception &ex)
  {
    string ss = string(ex.what());
    error_lable("[Get_output]");
    error_puts(ss.c_str());
    return "[FAILED]";
  }
}
inline string get_output(const Msg_type &type, const char *msg)
{
  Reg_thr(*rthr) = new Reg_thr;
  return get_output(type, msg, rthr);
}

string output_msg_chg(const Msg_type &type, string content)
{
  func_show("output_msg_chg");
  // cout<<content<<endl;
  regex reg(R"(\{%(.*?)\})");
  // string content=ans_gp[i].opt();
  smatch m;
  // content=str_strchg("\n"," ",content.c_str());
  // content=str_strchg("\r"," ",content.c_str());
  auto pos = content.cbegin();
  auto end = content.cend();
  int counts = 0;
  for (; std::regex_search(pos, end, m, reg);)
  {
    // cout<<m.str(1)<<endl;
    counts++;
    content = string(m.prefix()) + get_output(type, m.str(1).c_str()) + string(m.suffix());
    pos = content.cbegin();
    end = content.cend();
    if (counts > 20)
      return "[Too many to replace]!";
  }

  reg = regex(R"(\{\[(.*?)\]\})");
  pos = content.cbegin();
  end = content.cend();
  for (; std::regex_search(pos, end, m, reg);)
  {
    // cout<<m.str(1)<<endl;
    counts++;
    // string strs=m.str(1);
    Msg_type mtype = type;
    mtype.msgs = m.str(1);
    content = string(m.prefix()) + get_output(mtype, m.str(1).c_str()) + string(m.suffix());
    pos = content.cbegin();
    end = content.cend();
    if (counts > 20)
      return "[Too many to replace]!";
  }

  return content;
}

/*void Thr_reg(const char *txt, const regex &reg, bool *flg, Reg_thr *rthr)
{
  auto ret = regex_search(txt, reg);
  rthr->condWait.notify_one();
  (*flg) = ret;
}*/
/*int_fast8_t spj_reg(const char *txt, const regex &reg, Reg_thr *rthr)
{
  try
  {
    bool flgs;
    int thr = Main_thr_num.get_new_num();
    Main_thread_list[thr] = thread((Thr_reg), (txt), (reg), (&flgs), rthr);
    thread &Regs = Main_thread_list[thr];
    std::unique_lock<std::mutex> lockWait(rthr->mutexWait);
    std::cv_status cvsts = rthr->condWait.wait_for(lockWait, std::chrono::milliseconds(150));
    if (cvsts == std::cv_status::timeout)
    {
      Regs.join();
      return -1;
    }
    else
    {
      Regs.join();
      return flgs;
    }
    return 0;
  }
  catch (std::exception &ex)
  {
    string ss = string(ex.what());
    error_lable("[Regex]");
    error_puts(ss.c_str());
    return 0;
  }
}*/

inline bool reg_str_check(const char *msg, const string &rstr)
{
  regex reg(rstr);
  return regex_search(msg, reg);
}
bool reg_or_check(const char *msg, const json &regs);
bool reg_and_check(const char *msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (!reg_or_check(msg, regs[i]))
        return 0;
    }
    return 1;
  }
  else if (regs.is_object())
    return !reg_or_check(msg, regs["not"]);
  else
    return reg_str_check(msg, string(regs));
}
bool reg_or_check(const char *msg, const json &regs)
{
  if (regs.is_array())
  {
    int n = regs.size();
    for (int i = 0; i < n; i++)
    {
      if (reg_and_check(msg, regs[i]))
        return 1;
    }
    return 0;
  }
  else if (regs.is_object())
    return !reg_or_check(msg, regs["not"]);
  else
    return reg_str_check(msg, string(regs));
}
void reg_start(const char *msg, const json &regs, bool *flgs, Reg_thr *rthr)
{
  *flgs = reg_or_check(msg, regs);
  rthr->condWait.notify_one();
}

bool Ans_grp::check(const Msg_type &type, const char *msg)
{
  if (!type_check(type))
    return 0;
  if (strlen(msg) > (1 << 12))
    return 0;
  return reg_or_check(msg, jreg);
}

bool Ans_grp::type_check(const Msg_type &type)
{
  if (user[0] == "-99" || user[0] == "99")
    ifpriv = 1;
  else
    ifpriv = 0;
  int n;
  if (ifpriv)
  {
    if (!CHECK)
      return 0;
    if (user[0] == "-99")
    {
      n = user.size();
      for (int i = 1; i < n; i++)
      {
        if (user[i] == type.sender_id)
          return 0;
      }
    }
    // goto black_check;
  }
  if (type.ifgrp)
  {
    n = grps.size();
    if (grps[0] == "0")
      return 0;
    if (grps[0] == "1")
    {
      if (user[0] == "1" || user[0] == "0")
        return 1;
      else
        goto private_check;
    }
    if (grps[0] == "-1")
    {
      for (int i = 1; i < n; i++)
      {
        if (grps[i] == type.grp_id)
          return 0;
      }
      if (user[0] == "1" || user[0] == "0")
        return 1;
      goto private_check;
    }
    else
    {
      bool flgs = 0;
      for (int i = 0; i < n; i++)
      {
        if (grps[i] == type.grp_id)
        {
          flgs = 1;
          break;
        }
      }
      if (!flgs)
        return 0;
      if (user[0] == "1" || user[0] == "0")
        return 1;
      goto private_check;
    }
  }
  {

    if (user[0] == "0")
      return 0;
    if (user[0] == "1")
      return 1;
  private_check:
    if (ifpriv)
      return 1;
    bool flgs = user[0] == "-1";
    n = user.size();
    for (int i = 0; i < n; i++)
    {
      if (user[i] == type.sender_id)
        return !flgs;
    }
    return flgs;
  }
}

string chg_resend(Msg_type type, const char *msg)
{
  string str = get_output(type, msg);
  return (str == "[FAILED]") ? "" : str;
}

#endif
void Ans_init(const json &js)
{
  int n = js.size();
  for (int i = 0; i < n; i++)
  {
    Ans_grp tmp;
    tmp.get_init(js[i]);
    ans_gp.push_back(tmp);
  }
  sort(ans_gp.begin(), ans_gp.begin() + n);
}
void Ans_grp::get_init(const json &js)
{
  pri = js["pri"];

  grps = js["grps"];
  user = js["user"];
  anss = js["anss"];

  // reg_str = js["regex"];
  jreg = js["regex"];

  cnt = anss.size();
  lvs.resize(cnt + 1);
  int t;
  for (int i = 0; i < cnt; i++)
  {
    t = 1;
    if (get_st(anss[i].c_str(), "::") == 0)
    {
      sscanf(anss[i].c_str(), "::%d::", &t);
      anss[i] = anss[i].c_str() + (get_st(anss[i].c_str() + 2, "::") + 4);
    }
    lvs[i + 1] = lvs[i] + t;
  }
}
json Ans_grp::fexport()
{
  json rt;
  rt["pri"] = pri;
  rt["regex"] = jreg;

  rt["grps"] = grps;
  rt["user"] = user;
  rt["anss"] = anss;
  return rt;
}
json Fexport()
{
  json ss;
  for (int i = 0; i < ans_gp.size(); i++)
  {
    ss["Ans_grp"][i] = ans_gp[i].fexport();
  }
  return ss;
}

#endif
