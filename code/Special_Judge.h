#ifndef SPECIAL_JUDGE
#define SPECIAL_JUDGE
#include <string>
#include "output.h"
#include "File_Read.h"
#include "Jie_Meng_Base.h"
#ifndef LINUX_V
#include "HttpPost.h"
#include "AI_Draw2.h"
#endif
#include "Game.h"
#include <map>
#include <sstream>
#include <stdlib.h>
#include "md5.h"
#include "Note.h"
#include "Weather_Report.h"
#include "Encrypt.h"
#include "Spj_Ans.h"

using namespace std;
void resend(Msg_type type, const char *msg)
{
  func_show("resend");
  if (spj(type, msg))
    return;
  string str = get_output(type, msg);
  if (str != "[FAILED]")
    return output(type, str.c_str());
  // if(!type.ifgrp)return resend(type,"[default]");
  return;
}
map<string, Connect_Four> game_list;
string lpscq(const Msg_type &);
bool execmd(const char *cmd, char *result);
/**********Special Send***************/
void send_poke(Msg_type type, char *target_id)
{
  string msg("[CQ:poke,qq=");
  string id(target_id);
  msg = msg + id;
  msg = msg + "]";
  output(type, msg.c_str());
  /*for(int i=0;i<grp_num;i++){
    if(grp[i].crt(type,"[poke]")){
      grp[i].print(type);
      return;
    }
  }*/
}
string time_cal(const char *);

/**********Special Judge****************/
bool spj_poke(const char *msg)
{
  string aa("\"target_id\":");
  aa = aa + Self_ID;
  return get_st(msg, "\"sub_type\":\"poke\"") != -1 && get_st(msg, aa.c_str()) != -1;
}
bool no_str_spj(Msg_type type, const char *msg)
{
  func_show("no_str_spj");
  if (main_switch)
  {
    char *temp = new char[100];
    char temp2[10];
    memset(temp, 0, sizeof(char[100]));
    memset(temp2, 0, sizeof temp2);
#ifndef LINUX_V
    if (get_st(msg, "[msgs:") == 0)
    {
      int l, r, sltm, cnt = 0;
      l = strlen("[msgs:");
      char *buf = new char[1 << 12];
      sprintf(buf, "{\"group_id\": %s,\"messages\": [", type.grp_id);
      while (1)
      {
        if (msg[l] == '~')
        {
          l++;
          char *date = new char[16];
          char *id = new char[16];
          char *name = new char[1 << 7];
          char *text = new char[1 << 12];
          r = get_st(msg + l, "#") + l;
          get_copy(l, r, msg, date);

          l = r + 1;
          r = get_st(msg + l, "#") + l;
          get_copy(l, r, msg, id);
          l = r + 1;
          r = get_st(msg + l, "#") + l;
          get_copy(l, r, msg, name);
          l = r + 1;
          r = (get_st(msg + l, "~") == -1 ? (strlen(msg + l) - 1) : get_st(msg + l, "~")) + l;
          get_copy(l, r, msg, text);
          l = r;
          string tmm;
          int tnm = Main_thr_num.get_new_num();
          Main_thread_list[tnm] = thread(time_cal, date);
          Main_thread_list[tnm].join();
          {
            if (cnt)
              sprintf(buf + strlen(buf), ",");
            sprintf(buf + strlen(buf), "{\"type\": \"node\",\"data\": {");
            sprintf(buf + strlen(buf), "\"name\": \"%s\",", name);
            sprintf(buf + strlen(buf), "\"uin\": \"%s\",", id);
            sprintf(buf + strlen(buf), "\"content\": \"%s\",", text);
            sprintf(buf + strlen(buf), "\"time\": \"%s\"}}", tmm.c_str());
            // printf("%s\n",buf);
            cnt++;
          }
          delete[] date;
          delete[] id;
          delete[] name;
          delete[] text;
          delete[] buf;
        }
        else
        {
          sprintf(buf + strlen(buf), "]}");

          break;
        }
      }
      HttpRequest httpReq("127.0.0.1", OUTPORT);
      httpReq.HttpPost("/send_group_forward_msg", buf);
      return 1;
    }
#endif

    if (string(msg) == "[Add_Grp_Whitelist]")
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[100] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      Grp_white_list[white_list_num++] = sstr;
      return 1;
    }
    if (string(msg) == "[Del_Grp_Whitelist]")
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[100] = {0};
      bool flgs = 0;
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      for (int i = 1; i < white_list_num; i++)
      {
        if (Grp_white_list[i] == sstr)
        {
          Grp_white_list[i] = Grp_white_list[--white_list_num];
          flgs = 1;
          break;
        }
      }
      if (!flgs)
      {
        error_lable("[Spj_Delete]");
        error_puts("目标不存在");
      }
      return 1;
    }
    if (string(msg) == "[Add_Pri_Whitelist]")
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[100] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      Pri_white_list[Pri_white_list_num++] = sstr;
      return 1;
    }
    if (string(msg) == "[Del_Pri_Whitelist]")
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[100] = {0};
      bool flgs = 0;
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      for (int i = 0; i < Pri_white_list_num; i++)
      {
        if (Pri_white_list[i] == sstr)
        {
          Pri_white_list[i] = Pri_white_list[--Pri_white_list_num];
          flgs = 1;
          break;
        }
      }
      if (!flgs)
      {
        error_lable("[Spj_Delete]");
        error_puts("目标不存在");
      }
      return 1;
    }
    if (string(msg) == "[Add_Blacklist]")
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[100] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      black_list[black_list_num++] = sstr;
      return 1;
    }
    if (string(msg) == "[Del_Blacklist]")
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[100] = {0};
      bool flgs = 0;
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      for (int i = 0; i < black_list_num; i++)
      {
        if (black_list[i] == sstr)
        {
          black_list[i] = black_list[--black_list_num];
          flgs = 1;
          break;
        }
      }
      if (!flgs)
      {
        error_lable("[Spj_Delete]");
        error_puts("目标不存在");
      }
      return 1;
    }
    if (string(msg) == "[Save_Config]")
    {
      output(type, save_config() ? "Save Success!" : "Save Failed.");
      return 1;
    }

    if (spj_pfct("[7d_weather]", msg))
    {
      if (AL_TOKEN.size() < 2)
        return 0;
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        resend(type, "[WEATHER_INVALID]");
        return 1;
      }
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      char *str = acl_url_encode(sstr);
      string a;
      a = "curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/weather/forecast?location=";
      a = a + str + "&token=" + AL_TOKEN + "\"";
      // debug_puts(a.c_str());
      char bf[1 << 15] = {0};
      execmd(a.c_str(), bf);
      // debug_puts(bf);
      // puts(bf);
      City city;
      city.city_7d_init(bf);
      output(type, city._7dprint().c_str());
      if (IF_HAVE_MATLAB)
      {
        city.get_7dplot();
        output(type, city.outplot().c_str());
      }
      return 1;
    }
    if (spj_pfct("[24h_weather]", msg))
    {
      if (AL_TOKEN.size() < 2)
        return 0;
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        resend(type, "[WEATHER_INVALID]");
        return 1;
      }
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      char *str = acl_url_encode(sstr);
      string a;
      a = "curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/weather/hourly?location=";
      a = a + str + "&token=" + AL_TOKEN + "\"";
      puts(a.c_str());
      char bf[1 << 15] = {0};
      execmd(a.c_str(), bf);
      // puts(bf);
      City city;
      city.city_24h_init(bf);
      output(type, city._24hprint().c_str());
      if (IF_HAVE_MATLAB)
      {
        city.get_24hplot();
        output(type, city.outplot().c_str());
      }
      return 1;
    }
#ifndef LINUX_V
    if (spj_pfct("[MAT_RUN]", msg))
    {
      type.ord_lv = -1;
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1 << 13] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      FILE *fin;
      fin = fopen("mat_code.m", "w");
      fprintf(fin, "%s", sstr);
      fclose(fin);
      string comm = "mat_run.exe";
      // comm=comm+sstr+"\"";
      system(comm.c_str());
      FILE *fp;
      fp = fopen("outtmp.txt", "r");
      char strText[1 << 12] = {0};
      char sstrt[1 << 8];
      while (fgets(sstrt, 1 << 8, fp) != NULL)
        sprintf(strText + strlen(strText), "%s", sstrt);
      if (strlen(strText) < 1)
        sprintf(strText, "OK!");
      output(type, strText);
      fclose(fp);
      fp = fopen("outtmp.txt", "w");
      fprintf(fp,"");
      fclose(fp);
      //system("del outtmp.txt");
      return 1;
    }
#endif

    if (spj_pfct("[C++_RUN]", msg))
    {
      type.ord_lv = -1;
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1 << 13] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      FILE *fi;
      fi = fopen("test.cpp", "w");
#ifndef LINUX_V
      fprintf(fi, "#include<stdio.h>\n#include<stdlib.h>\n#include<thread>\n#include<windows.h>\n");
      fprintf(fi, "void calc();\n");
      fprintf(fi, "void tm(){Sleep(200);printf(\"\\nTLE\\n\");exit(1);}\n");
#else
      fprintf(fi, "#include<stdio.h>\n#include<stdlib.h>\n#include<thread>\n#include <unistd.h>\n");
      fprintf(fi, "void calc();\n");
      fprintf(fi, "void tm(){usleep(200000);printf(\"\\nTLE\\n\");exit(1);}\n");
#endif
      fprintf(fi, "int main(){std::thread t(tm);t.detach();calc();return 0;}\n");

      fprintf(fi, "%s", sstr);
      fclose(fi);
      char buff[1 << 16] = {0};

      execmd("g++ 2>&1 test.cpp -o test.exe", buff);
      // char buff[1<<16]={0};
      execmd("test.exe", buff + strlen(buff));

      output(type, buff);

      return 1;
    }
    if (spj_pfct("[PYTHON_RUN]", msg))
    {
      type.ord_lv = -1;
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1 << 13] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      STR_DECODE(sstr);
      strchg("\n", "\n  ", sstr);
      FILE *fi;
      fi = fopen("test.py", "w");
      fprintf(fi, "import time\nfrom func_timeout import func_set_timeout\n\n");
      fprintf(fi, "@func_set_timeout(3)\ndef task():\n");
      fprintf(fi, "%s\n", sstr);
      fprintf(fi, "if __name__ == '__main__':\n  task()");
      fclose(fi);
      char buff[1 << 16] = {0};

      execmd("python 2>&1 test.py", buff);
      // char buff[1<<16]={0};
      // execmd("test.exe",buff+strlen(buff));
      if (strlen(buff) == 0)
      {
        output(type, "OK!");
        return 1;
      }
#ifndef LINUX_V
      output(type, GBKToUTF8(buff).c_str());
#else
      output(type, buff);
#endif
      return 1;
    }
    if (spj_pfct("[R_RUN]", msg))
    {
      type.ord_lv = -1;
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1 << 13] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      strchg("\r", "", sstr);
      FILE *fi;
      fi = fopen("test.r", "w");
      fprintf(fi, "require('R.oo',quietly=TRUE,warn.conflicts=FALSE)\nrequire('R.utils',quietly=TRUE,warn.conflicts=FALSE)\n");
      fprintf(fi, "fexe <- function() {");
      fprintf(fi, "%s\n", sstr);
      fprintf(fi, "}\nres <- withTimeout({\n  fexe()\n}, timeout = 3, onTimeout = \"silent\")");
      fclose(fi);
      char buff[1 << 16] = {0};

      execmd("Rscript 2>&1 test.r", buff);

      if (strlen(buff) == 0)
      {
        output(type, "OK!");
        return 1;
      }

      output(type, buff);

      return 1;
    }
    if (spj_pfct("[get_pic]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      // if(l==-1){output(type,"错误：未指定图片名称");return 1;}
      int r = strlen(type.msgs.c_str());
      char str[1024];
      get_copy(l + 1, r, type.msgs.c_str(), str);
      string back_x = str + strlen(str) - 3;
      if (back_x != "bmp" && back_x != "png" && back_x != "jpg")
      {
        output(type, "不支持的格式");
        return 1;
      }
      char file_path[1 << 9];
      memset(file_path, 0, sizeof file_path);
      getcwd(file_path, sizeof file_path);
      string fpath = "";
      strchg("\\", "/", file_path);
      string rt = rt + "[CQ:image,file=file:///" + file_path + "/" + str + "]";
      type.msgs = "[OUTPUT_JOIN]";
      output(type, rt.c_str());
      fpath = fpath + "del " + str;
      system(fpath.c_str());
      return 1;
    }
    if (spj_pfct("[delete_msg]", msg))
    {
      int l = get_st(type.msgs.c_str(), "[CQ:reply,id=");
      l += strlen("[CQ:reply,id=") - 1;
      // if(l==-1){output(type,"错误：未指定图片名称");return 1;}
      int r = get_st(type.msgs.c_str(), "]");
      char str[19];
      get_copy(l + 1, r, type.msgs.c_str(), str);
      char pt[6];
      sprintf(pt, "%d", OUTPORT);
      string commd = "curl -s \"";
      commd = commd + IP_ADD + "delete_msg?message_id=" + str + "\"";
      // debug_puts(commd.c_str());
      system(commd.c_str());
      return 1;
    }

    if (spj_pfct("[Connect_Four_Start]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        output(type, "错误：未指定棋局名称");
        return 1;
      }
      int r = strlen(type.msgs.c_str());
      char str[1024];
      get_copy(l + 1, r, type.msgs.c_str(), str);
      string a(str);
      Connect_Four aa;
      game_list[a] = aa;
      output(type, game_list[a].opr("start").c_str());
      // resend(type,a.c_str());
      return 1;
    }

#ifdef AI_DRAW

    if (spj_pfct("[AI]", msg))
    {
      return AI_Draw(type);
    }

#endif

    if (spj_pfct("[CHG_NOTE]", msg))
    {
      if (!chg_note(type.msgs.c_str()))
        resend(type, "[NOTE_ERROR]");
      return 1;
    }
    if (spj_pfct("[GET_NOTE]", msg))
    {
      output(type, get_note(type.msgs.c_str()).c_str());
      return 1;
    }
    if (spj_pfct("[SAVE_NOTE]", msg))
    {
      if (!write_note())
        resend(type, "[NOTE_ERROR]");
      resend(type, "[SAVE_SUCCESS]");
      return 1;
    }
    if (spj_pfct("[CHEM]", msg))
    {
      Chem(type);
      return 1;
    }

    if (spj_pfct("[Connect_Four]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        output(type, "错误：未指定棋局名称");
        return 1;
      }
      int r = get_st(type.msgs.c_str() + l + 1, "#");
      if (r == -1)
      {
        output(type, "[错误指令]");
        return 1;
      }
      r += l + 1;
      char str[1024];
      get_copy(l + 1, r, type.msgs.c_str(), str);
      string a(str);
      if (game_list.find(a) == game_list.end())
      {
        output(type, "错误：不存在该棋局");
        return 1;
      }
      l = r;
      r = strlen(type.msgs.c_str());
      get_copy(l + 1, r, type.msgs.c_str(), str);
      // string b(str);
      string optt;
      optt = "棋局" + a + "\n";
      output(type, (optt + game_list[a].opr(str)).c_str());
      // resend(type,a.c_str());
      return 1;
    }
    if (spj_pfct("[Group_Exit]", msg) && type.ifgrp)
    {
      char ss[1 << 5] = {0};
      sprintf(ss, "%d", OUTPORT);
      string coms = "curl -s \"http://127.0.0.1:";
      coms = coms + ss + "/set_group_leave?group_id=" + type.grp_id + "\"";
      system(coms.c_str());
      return 1;
    }

    if (get_st(msg, ".type_repost") == 0 && (CHECK))
    {
      char tmp[1 << 13] = {0};
      Msg_type ntype;
      // ntype.msgs=type.msgs;
      int l, r;
      l = get_st(msg, "#");
      r = get_st(msg + l + 1, "#") + l + 1;
      get_copy(l + 1, r, msg, tmp);
      if (r - l < 3)
        ntype.ifgrp = 0;
      else
        ntype.ifgrp = 1;
      ntype.grp_id = tmp;
      // get_copy(0,strlen(tmp),tmp,ntype.grp_id.c_str());
      l = r + 1;
      r = get_st(msg + l + 1, "#") + l + 1;
      memset(tmp, 0, sizeof tmp);

      get_copy(l, r, msg, tmp);
      ntype.sender_id = tmp;
      // debug_puts("type_repost");
      // cout<<msg<<endl;
      // ntype.show();
      if (ntype.ifgrp && string(ntype.grp_id) == "WHITE_LIST")
      {
        for (int i = 1; i < white_list_num; i++)
        {
          Msg_type stype = ntype;
          stype.ord_lv = 100;
          stype.grp_id = Grp_white_list[i];
          char nmsg[1 << 13];
          strcpy(nmsg, msg);
          strchg("WHITE_LIST", Grp_white_list[i].c_str(), nmsg);
          // debug_puts(nmsg);
          thread Respjthr = thread(spj, stype, nmsg);
          Respjthr.detach();
          uni_sl(delay_time);
        }
        return 1;
      }
      if (string(ntype.sender_id) == "BROAD")
      {
        for (int i = 0; i < broad_pri_num; i++)
        {
          Msg_type stype = ntype;
          stype.ord_lv = 100;
          stype.sender_id = broad_pri[i];
          char nmsg[1 << 13];
          strchg("BROAD", broad_pri[i].c_str(), nmsg);
          thread Respjthr = thread(spj, stype, nmsg);
          Respjthr.detach();
          uni_sl(delay_time);
        }
        return 1;
      }
      // cout<<msg<<endl;
      // char stmps[1<<13]={0};
      // sprintf(tmp,"%s",msg+r+1);
      get_copy(r + 1, strlen(msg), msg, tmp);

      strchg("&#91;", "[", tmp);
      strchg("&#93;", "]", tmp);
      strchg("&amp;", "&", tmp);

      output(ntype, tmp);
      return 1;
    }
    if (get_st(msg, ".type_resend") == 0 && (CHECK))
    {

      char tmp[1000];
      Msg_type ntype;

      memset(tmp, 0, sizeof tmp);
      int l, r;
      l = get_st(msg, "#");
      r = get_st(msg + l + 1, "#") + l + 1;
      get_copy(l + 1, r, msg, tmp);
      if (r - l < 3)
        ntype.ifgrp = 0;
      else
        ntype.ifgrp = 1;
      ntype.grp_id = tmp;
      // get_copy(0,strlen(tmp),tmp,ntype.grp_id.c_str());
      l = r + 1;
      r = get_st(msg + l + 1, "#") + l + 1;
      memset(tmp, 0, sizeof tmp);
      get_copy(l, r, msg, tmp);
      ntype.sender_id = tmp;

      if (ntype.ifgrp && ntype.grp_id == "WHITE_LIST")
      {
        for (int i = 1; i < white_list_num; i++)
        {
          Msg_type stype = ntype;
          stype.ord_lv = 100;
          stype.grp_id = Grp_white_list[i];
          char nmsg[1 << 13];
          strcpy(nmsg, msg);
          strchg("WHITE_LIST", Grp_white_list[i].c_str(), nmsg);
          // debug_puts(nmsg);
          int thr_num = Main_thr_num.get_new_num();
          Main_thread_list[thr_num] = thread(spj, stype, nmsg);
          Main_thread_list[thr_num].detach();

          /*thread Respjthr=thread(spj,stype,nmsg);
          Respjthr.detach();*/

          uni_sl(delay_time);
        }
        return 1;
      }
      if (string(ntype.sender_id) == "BROAD")
      {
        for (int i = 0; i < broad_pri_num; i++)
        {
          Msg_type stype = ntype;
          stype.ord_lv = 100;
          stype.sender_id = broad_pri[i];
          char nmsg[1 << 13];
          strchg("BROAD", broad_pri[i].c_str(), nmsg);
          int thr_num = Main_thr_num.get_new_num();
          Main_thread_list[thr_num] = thread(spj, stype, nmsg);
          Main_thread_list[thr_num].detach();
          uni_sl(delay_time);
        }
        return 1;
      }

      get_copy(r + 1, strlen(msg), msg, tmp);
      strchg("&#91;", "[", tmp);
      strchg("&#93;", "]", tmp);
      strchg("&amp;", "&", tmp);
      resend(ntype, tmp);

      return 1;
    }
    if (spj_pfct(".print", msg) && (CHECK) && !type.ifgrp)
    {
      freopen(ANS_FILENAME.c_str(), "r", stdin);
      char txt[4000];
      memset(txt, 0, sizeof txt);
      char opt[10000];
      memset(opt, 0, sizeof opt);
      while (txt[0] != '/')
      {
        memset(opt, 0, sizeof opt);
        for (int i = 0; i < 15 && txt[0] != '/'; i++)
        {
          memset(txt, 0, sizeof txt);
          gets(txt);
          get_copy(0, strlen(txt), txt, opt + strlen(opt));
          get_copy(0, 4, "[\\n]", opt + strlen(opt));
        }
        output(type, opt);
      } // fclose(stdin);
      return 1;
    }
  }

  if (spj_pfct(".switch on", msg) && (CHECK))
  {
    main_switch = 1;
    return 1;
  }
  if (spj_pfct(".switch off", msg) && (CHECK))
  {
    main_switch = 0;
    return 1;
  }


  return 0;
}
bool spj(const Msg_type &type /*,const char *buf*/, const char *msg)
{

  string opt_rt = str_spj(type, msg);
  if (opt_rt != "[SPJ_CONTINUE]")
  {
    output(type, opt_rt.c_str());
    return 1;
  }
  return no_str_spj(type, msg);

}

typedef long long ll;
string time_cal(const char *date)
{
  puts(date);
  int y, m, d, h, min, s, deld;
  char yy[5] = {0};
  char mm[5] = {0};
  char dd[5] = {0};
  char hh[5] = {0};
  char nn[5] = {0};
  char ss[5] = {0};
  get_copy(0, 4, date, yy);
  get_copy(4, 6, date, mm);
  get_copy(6, 8, date, dd);
  get_copy(8, 10, date, hh);
  get_copy(10, 12, date, nn);
  get_copy(12, 14, date, ss);
  sscanf(yy, "%d", &y);
  sscanf(mm, "%d", &m);
  sscanf(dd, "%d", &d);
  sscanf(hh, "%d", &h);
  sscanf(nn, "%d", &min);
  sscanf(ss, "%d", &s);
  ll tot_time = 0;
  int ddd[13] = {0, -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333};
  deld = (y - 1970) * 365 + (y - 1968) / 4 - (y % 4 == 0 && m <= 2) + d + ddd[m];
  tot_time = (ll)(deld)*24L * 3600L + (ll)(h - 8) * 3600L + (ll)(min * 60) + (ll)(s);
  char opt[1 << 5] = {0};
  sprintf(opt, "%lld", tot_time);
  string rt(opt);
  return rt;
}

#endif
