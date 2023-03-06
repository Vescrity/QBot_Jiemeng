#ifndef SPJ_ANS
#define SPJ_ANS
#include "opstring.h"
#include "Jie_Meng_Base.h"
#include "md5.h"
#ifndef LINUX_V
#include "HttpPost.h"
#endif
string lpscq(const Msg_type &);
bool execmd(const char *cmd, char *result);
string run_cmd(Msg_type type);
string status_msgs();
string txt2img(const char *txt)
{
  func_show("txt2img");
  char *ff = acl_url_encode(txt);
  string rt(ff);
  free(ff);
  //string s = rt;
  string opt = R"(curl -s "http://api.setbug.com/tools/text2image/?text=)";
  opt = opt + rt + R"(" --output "text.png")";
  system(opt.c_str());
  rt="[CQ:image,file=file:///";
  rt=rt+RUN_PATH+"/text.png]";
  
  return rt;
}
string tags(const char *txt, int l, const char c)
{
  string rt;
  char s[2];
  rt = "\0";
  s[1] = 0;
  rt += c;
  rt += "tag=";
  for (int i = l + 1; i < strlen(txt); i++)
  {
    if (txt[i] == '#')
    {
      rt += "&tag=";
      continue;
    }
    if (txt[i] == ']')
      break;
    s[0] = txt[i];
    rt += acl_url_encode(s);
  }
  rt += "\0";
  return rt;
}
bool spj_rand(const char *msg)
{
  return pfct(msg, ".rd");
}
bool spj_pfct(const char *txxt, const char *msg)
{
  return pfct(msg, txxt) && strlen(msg) == strlen(txxt);
}
string Chem(Msg_type type);
string str_spj(const Msg_type &type, const char *msg)
{
  func_show("str_spj");
  if (main_switch)
  {
    if (spj_pfct(".info", msg))
    {
      string vinfo = "";
      vinfo = vinfo + CURRENT_VERSION +
              "\n更新于" + UPDATE_TIME + "[\\n]"
                                         "开发by雪と葉[\\n]"
                                         "项目地址:https://github.com/Vescrity/QBot_Jiemeng[\\n]"
                                         "Special thanks@AiCorein https://github.com/AiCorein[\\n]"
                                         "请勿滥用bot酱呐♥";
      return vinfo;
    }
    if (get_st(msg, "[plugin:") == 0)
    {
      int l = 8;
      int r = strlen(msg) - 1;
      char pl_name[1 << 7] = {0};
      get_copy(l, r, msg, pl_name);
#ifdef LINUX_V
      string commd = "./plugin/";
      commd = commd + pl_name + "/" + pl_name;
#else
      string commd = ".\\plugin\\";
      commd = commd + pl_name + "\\" + pl_name + ".exe";
#endif
      info_lable("[plugin]");
      info_print("调用插件");
      info_puts(pl_name);
      FILE *fp;
      fp = fopen("Current_msg.txt", "w");
      fprintf(fp, "%s", type.msgs.c_str());
      fclose(fp);
      char buf[1 << 13] = {0};
      execmd(commd.c_str(), buf);
      return buf;
    }
    if (spj_pfct("[moe_lp]", msg))
    {
      return lpscq(type);
    }
    if (spj_pfct("[txt2img]", msg))
    {
      int l, r;
      l = get_st(type.msgs.c_str(), "#");
      r = strlen(type.msgs.c_str());
      if (l == -1)
        return "[FAILED]";
      char *txts = new char[1 << 13];
      get_copy(l + 1, r, type.msgs.c_str(), txts);
      string rt = txt2img(txts);
      delete[] txts;
      return rt;
    }

    if (spj_pfct("[get_163]", msg))
    {
      int l, r;
      l = get_st(type.msgs.c_str(), "#");
      r = strlen(type.msgs.c_str());
      if (l == -1)
        return 0;
      char name[128], sid[20];
      memset(name, 0, sizeof name);
      memset(sid, 0, sizeof sid);
      get_copy(l + 1, r, type.msgs.c_str(), name);
      char *ff = acl_url_encode(name);
      string rt(ff);
      free(ff);
      // puts(rt.c_str());
      string ord = "curl -s ";
      string url = "\"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
      url = url + rt + "&type=1&offset=0&total=true&limit=1\"";
      char bf[2048];
      memset(bf, 0, sizeof bf);
      ord = ord + url;
      execmd(ord.c_str(), bf);
      l = get_st(bf, "[{\"id\":");
      r = get_st(bf + l, ",\"");
      r += l;
      get_copy(l + 7, r, bf, sid);
      string opt = "[163:";
      opt = opt + sid + ",qwq]";
      return opt;
    }
    if (spj_pfct("[rec_163]", msg))
    {
      int l, r;
      l = get_st(type.msgs.c_str(), "#");
      r = strlen(type.msgs.c_str());
      if (l == -1)
        return 0;
      char name[128], sid[20];
      memset(name, 0, sizeof name);
      memset(sid, 0, sizeof sid);
      get_copy(l + 1, r, type.msgs.c_str(), name);
      char *ff = acl_url_encode(name);
      string rt(ff);
      free(ff);
      // puts(rt.c_str());
      string ord = "curl -s ";
      string url = "\"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=";
      url = url + rt + "&type=1&offset=0&total=true&limit=1\"";
      char bf[2048];
      memset(bf, 0, sizeof bf);
      ord = ord + url;
      execmd(ord.c_str(), bf);
      l = get_st(bf, "[{\"id\":");
      r = get_st(bf + l, ",\"");
      r += l;
      get_copy(l + 7, r, bf, sid);
      string opt = "[CQ:record,file=http://music.163.com/song/media/outer/url?id=";
      opt = opt + sid + ".mp3]";
      return opt;
    }
    if (get_st(msg, "[lolicon_setu") == 0)
    {
      int l, r;
      char bf[2048];
      memset(bf, 0, sizeof bf);
      string str, str2;
      str = "curl -s \"https://api.lolicon.app/setu/v2";
      str2 = tags(msg, 13, '?');
      str = str + str2 + "\"";
      // puts(str.c_str());
      execmd(str.c_str(), bf);
      l = get_st(bf, "{\"original\"");
      r = get_st(bf + l, "\"}");
      if (l == -1)
      {
        // resend(type,"[Not Found]");
        return "{%[Not Found]}";
      }
      r += l;
      l += 13;
      string opt = "[CQ:image,file=";
      char furl[512];
      memset(furl, 0, sizeof furl);
      get_copy(l, r, bf, furl);
      // puts(furl);
      opt = opt + furl + "]";
      return opt;
    }
    if (spj_pfct("[lolicon_sotu]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "{%[lolicon_setu]}";
      }
      int r = strlen(type.msgs.c_str());
      char str[1024];
      get_copy(l, r, type.msgs.c_str(), str);
      string a;
      a = "{%[lolicon_setu";
      a += str;
      a += "]}";
      return a;
    }
    if (spj_pfct("[r18_sotu]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "{%[r18_setu]}";
      }
      int r = strlen(type.msgs.c_str());
      char str[1024];
      get_copy(l, r, type.msgs.c_str(), str);
      string a;
      a = "{%[r18_setu";
      a += str;
      a += "]}";
      return a;
    }
    if (get_st(msg, "[r18_setu") == 0)
    {
      int l, r;
      char bf[2048];
      string str, str2;
      str = "curl -s \"https://api.lolicon.app/setu/v2";
      str2 = tags(msg, 9, '&');
      str = str + "?r18=1" + str2 + "\"";
      // puts(str.c_str());
      memset(bf, 0, sizeof bf);
      execmd(str.c_str(), bf);
      // puts(bf);
      l = get_st(bf, "{\"original\"");
      r = get_st(bf + l, "\"}");
      if (l == -1)
      {
        return "[Not Found]";
      }
      r += l;
      l += 13;
      string opt = "[CQ:image,file=";
      char furl[512];
      memset(furl, 0, sizeof furl);
      get_copy(l, r, bf, furl);
      // puts(furl);
      opt = opt + furl + "]";
      return opt;
    }
    if (spj_pfct("[sakura_setu]", msg))
    {
      int l, r;
      char bf[2048];
      memset(bf, 0, sizeof bf);
      execmd("curl -s https://www.dmoe.cc/random.php?return=json", bf);
      l = get_st(bf, "\"imgurl\":\"");
      r = get_st(bf + l + 15, "\"");
      r += l + 15;
      l += 10;
      // puts(bf);

      string opt = "[CQ:image,file=";
      char ffurl[512], furl[512];
      memset(ffurl, 0, sizeof ffurl);
      memset(furl, 0, sizeof furl);
      get_copy(l, r, bf, ffurl);
      // puts(ffurl);
      int j = 0;
      for (int i = 0; i < strlen(ffurl); i++)
        if (ffurl[i] != '\\')
          furl[j++] = ffurl[i];
      // puts(furl);
      opt = opt + furl + "]";
      return opt;
    }
    if (spj_pfct("[MirlKoi_setu]", msg))
    {
      int l, r;
      char bf[2048];
      memset(bf, 0, sizeof bf);
      execmd("curl -s \"https://iw233.cn/api.php?sort=random&type=json\"", bf);
      l = get_st(bf, "[\"");
      r = get_st(bf + l + 2, "\"");
      r += l + 2;
      l += 2;
      // puts(bf);

      string opt = "[CQ:image,file=";
      char ffurl[512], furl[512];
      memset(ffurl, 0, sizeof ffurl);
      memset(furl, 0, sizeof furl);
      get_copy(l, r, bf, ffurl);
      // puts(ffurl);
      int j = 0;
      for (int i = 0; i < strlen(ffurl); i++)
        if (ffurl[i] != '\\')
          furl[j++] = ffurl[i];
      // puts(furl);
      opt = opt + furl + "]";
      return opt;
    }
    if (spj_pfct("[tohou_setu]", msg))
    {
      int l, r;
      char bf[2048];
      memset(bf, 0, sizeof bf);
      execmd("curl -s \"https://img.paulzzh.com/touhou/random?type=json\"", bf);
      l = get_st(bf, "/image/");
      r = get_st(bf + l + 8, "/");
      r += l + 8;
      l += 7;
      // puts(bf);
      string opt = "[CQ:image,file=https://img.paulzzh.com/touhou/konachan/image/";
      char furl[512];
      memset(furl, 0, sizeof furl);
      get_copy(l, r, bf, furl);
      // puts(furl);
      opt = opt + furl + ".jpg]";
      return opt;
    }
    if (spj_pfct("[Xinhua]", msg))
    {
      if (AL_TOKEN.size() < 2)
        return 0;
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "{%[NONE]}";
      }
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      char *str = acl_url_encode(sstr);
      string a;
      a = "curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/word?word=";
      a = a + str + "&token=" + AL_TOKEN + "\"";
      // puts(a.c_str());
      char bf[1 << 15] = {0};
      execmd(a.c_str(), bf);
      // puts(bf);
      string py = json_read(bf, "pinyin");
      string exp = json_read(bf, "explanation");
      string op = op + sstr + "\n" + py + "\n" + exp;
      return op;
    }

    if (spj_pfct("[risk_area]", msg))
    {
      if (AL_TOKEN.size() < 2)
        return 0;
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      if ((get_st(sstr, "city=") == -1) && (get_st(sstr, "county=") == -1))
      {
        return "x";
      }
      char *str = acl_url_encode(sstr);
      strchg("%3D", "=", str);
      string a;
      a = "curl  --ssl-no-revoke -s \"https://v2.alapi.cn/api/springTravel/risk?";
      a = a + str + "&token=" + AL_TOKEN + "\"";
      // puts(a.c_str());
      char bf[1 << 18] = {0};
      execmd(a.c_str(), bf);
      // puts(bf);
      l = 0;
      char op[1 << 18] = "该地区高风险地区如下：\n";
      int l2 = 0;
      while ((l = get_st(bf + l2, "communitys")) != -1)
      {
        l = l2 + l + 14;
        r = get_st(bf + l, "]") - 1 + l;
        get_copy(l, r, bf, op + strlen(op));
        // puts(op);
        l2 = r + 1;
      }
      // printf("\ngetok!\n");
      // puts(op);
      strchg(",", "\n", op);
      strchg("\"", "", op);
      // puts(op);
      // output(type,op);
      return op;
    }
    if (spj_pfct("[baike]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      char *str = acl_url_encode(sstr);
      string a;
      a = "curl  --ssl-no-revoke -s \"https://api.wer.plus/api/dub?t=";
      a = a + str + "\"";
      // puts(a.c_str());
      char bf[1 << 13] = {0};
      // char imgs[1<<10];
      // char txxts[1<<15];
      execmd(a.c_str(), bf);
      // puts(bf);
      string txxts, imgs;
      txxts = json_read(bf, "text");
      imgs = json_read(bf, "img_url");
      string opts = sstr;
      opts = opts + "\n" + txxts + "\n[CQ:image,file=" + imgs + "]";
      // output(type,opts.c_str());
      return opts;
    }
    if (spj_pfct("[chat]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      char str[1 << 12] = {0};
      utf8tounicode(sstr, str);
      string commd = "curl -s https://api.openai.com/v1/completions -H "
                     "\"Content-Type: application/json\" "
                     "-H \"Authorization: Bearer " +
                     OPEN_AI_SECRET + "\" "
                                      "-d \"{\\\"model\\\": \\\"" +
                     OPEN_AI_MODEL + "\\\", \\\"prompt\\\": \\\"";
      commd = commd + str;
      commd = commd + "\\\", \\\"user\\\": \\\"";
      string md5_id = md5(string(type.sender_id));
      commd = commd + "Jiemeng" + md5_id;
      commd = commd + "\\\", \\\"temperature\\\": 0.3, \\\"top_p\\\": 1, \\\"max_tokens\\\": 1000}\"";
      char bf[1 << 15] = {0};
      execmd(commd.c_str(), bf);
      // color_puts(bf);
      string p = json_read(bf, "text");
      p = decode(p);
      // output(type,p.c_str());
      return p;
    }
    if (spj_pfct("[DAILY_NEWS]", msg))
    {
      string a;
      a = "curl  --ssl-no-revoke -s \"https://api.wer.plus/api/ndys\"";
      puts(a.c_str());
      char bf[1 << 13] = {0};
      char imgs[1 << 10];
      execmd(a.c_str(), bf);
      // puts(bf);
      int l, r;
      l = get_st(bf, "\"url\":") + 7;
      r = get_st(bf + l, "\"}") + l;
      get_copy(l, r, bf, imgs);
      string opts = "";
      opts = opts + "[CQ:image,file=" + imgs + "]";
      return opts;
    }
    if (spj_pfct("[dict]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "[DICT_INVALID]";
      }
      int r = strlen(type.msgs.c_str());
      char sstr[1024] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      char *str = acl_url_encode(sstr);
      string a;
      a = "curl  --ssl-no-revoke -s \"http://dict.youdao.com/suggest?num=1&doctype=json&q=";
      a = a + str + "\"";
      // puts(a.c_str());
      char bf[1 << 13] = {0};
      char means[1 << 10] = {0};

      execmd(a.c_str(), bf);
      // puts(bf);
      if (get_st(bf, "msg\":\"not found") != -1)
      {
        return "No Result.";
      }
      l = get_st(bf, "\"explain\":") + 11;
      r = get_st(bf + l, "\",\"entry") + l;
      get_copy(l, r, bf, means);

      string opts = sstr;
      opts = opts + "\n" + means + "\n来源：http://dict.youdao.com/";
      return opts;
    }
    if (spj_pfct("[trans]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "[DICT_INVALID]";
      }
      int r = strlen(type.msgs.c_str());
      char sstr[1 << 15] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      char *str = acl_url_encode(sstr);
      string a;
      a = "curl -s \"http://fanyi.youdao.com/translate?&doctype=json&type=AUTO&i=";
      a = a + str + "\"";
      // puts(a.c_str());
      char bf[1 << 15] = {0};
      char means[1 << 15] = {0};
      execmd(a.c_str(), bf);
      // puts(bf);
      l = r = 0;
      string opts = sstr;
      opts = opts + "\n";
      while (get_st(bf + r, "\"tgt\":\"") != -1)
      {
        l = get_st(bf + r, "\"tgt\":\"") + 7 + r;
        r = get_st(bf + l, "\"}") + l;
        get_copy(l, r, bf, means);
        opts = opts + means;
      }
      opts = opts + "\n来源：http://fanyi.youdao.com/";
      return opts;
    }
    if (spj_pfct("[Baidu_trans]", msg))
    {
      if (BAIDU_APP_SECRET.size() < 2)
        return 0;
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "[DICT_INVALID]";
      }
      int r = strlen(type.msgs.c_str());
      char sstr[1 << 15] = {0};
      get_copy(l + 1, r, type.msgs.c_str(), sstr);
      strchg("&#91;", "[", sstr);
      strchg("&#93;", "]", sstr);
      strchg("&amp;", "&", sstr);
      string q = sstr;
      string opt_lang;
      string salt = "SALTJIEMENG";
      string str_1 = BAIDU_APP_ID + q + salt + BAIDU_APP_SECRET;
      str_1 = md5(str_1);
      char *str = acl_url_encode(sstr);
      int l2 = get_st(type.msgs.c_str(), "~");
      if (l2 == -1)
      {
        opt_lang = "en";
      }
      else
      {
        r = l;
        get_copy(l2 + 1, r, type.msgs.c_str(), sstr);
        opt_lang = sstr;
      }
      string a;
      a = "curl -s \"https://fanyi-api.baidu.com/api/trans/vip/translate?from=auto&to=" + opt_lang +
          "&appid=" + BAIDU_APP_ID + "&salt=" + salt + "&sign=" + str_1 + "&q=";
      a = a + str + "\"";
      // puts(a.c_str());
      char bf[1 << 15] = {0};
      char means[1 << 15] = {0};
      execmd(a.c_str(), bf);
      // puts(bf);
      l = r = 0;
      string opts = q.c_str();
      opts = opts + "\n";
      while (get_st(bf + r, "\"dst\":\"") != -1)
      {
        l = get_st(bf + r, "\"dst\":\"") + 7 + r;
        r = get_st(bf + l, "\"}") + l;
        get_copy(l, r, bf, means);
        char *sss = unicode_chg(means);
        opts = opts + sss;
        delete sss;
      }
      return opts;
    }
#ifndef LINUX_V
    if (spj_pfct("[midigen]", msg))
    {
      char tempp[1 << 15];
      memset(tempp, 0, sizeof tempp);
      strcat(tempp, type.msgs.c_str());

      strchg("&#91;", "[", tempp);
      strchg("&#93;", "]", tempp);
      strchg("&amp;", "&", tempp);
      int l = get_st(tempp, "#");
      int r = strlen(tempp);
      char str[1 << 15];
      get_copy(l, r, tempp, str);
      HttpRequest httpReq("127.0.0.1", 8080);
      string res;
      res = httpReq.HttpPost("/api", HttpRequest::genJsonString("midercode", str));
      // printf("%s\n",res.c_str());
      l = get_st(res.c_str(), "\"stream\" : \"") + 12;
      r = get_st(res.c_str() + l, "\"") + l;
      char file_name[1 << 9];
      memset(file_name, 0, sizeof file_name);
      getcwd(file_name, sizeof file_name);
      get_copy(l, r, res.c_str(), file_name + strlen(file_name));
      // printf("%s\n",file_name);
      for (int i = 0; i < strlen(file_name); i++)
      {
        if (file_name[i] == '\\')
          file_name[i] = '/';
      }
      string opt = "[CQ:record,file=file:///";
      opt = opt + file_name + "]";
      return opt;
    }
#endif
    if (spj_pfct("[midigen_info]", msg))
    {
      string opt = "MiraiMidiProduce v0.1.8 By whiter\n";
      opt += "项目地址：https://github.com/whiterasbk/MiraiMidiProduce\n";
      opt += "参考指南：http://smmcat.top/?p=36\n";
      return opt;
    }

#ifndef LINUX_V
    if (spj_pfct("[get_screen]", msg))
    {
      system("nircmd savescreenshot Screen.png");
      string opt = "[CQ:image,file=file:///./Screen.png]";
      return opt;
    }
#endif

    /*********************计时器********************/
    if (spj_pfct("[tic]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "错误：未指定计时器名称";
      }
      int r = strlen(type.msgs.c_str());
      char str[1024];
      get_copy(l + 1, r, type.msgs.c_str(), str);
      string a;
      a = str;
      time_list[a] = clock();
      return "计时添加成功";
    }
    if (spj_pfct("[toc]", msg))
    {
      int l = get_st(type.msgs.c_str(), "#");
      if (l == -1)
      {
        return "错误：未指定计时器名称";
      }
      int r = strlen(type.msgs.c_str());
      char str[1024];
      get_copy(l + 1, r, type.msgs.c_str(), str);
      string a;
      a = str;
      if (time_list.find(a) == time_list.end())
      {
        return "错误：未指定计时器名称";
      }
      int dt = clock() - time_list[a];
      int sec, min, hr;
      dt /= 1000;
      sec = dt % 60;
      dt /= 60;
      min = dt % 60;
      dt /= 60;
      hr = dt;
      char opt[512];
      memset(opt, 0, sizeof opt);
      sprintf(opt, "计时器%s用时：%d:%02d:%02d", a.c_str(), hr, min, sec);
      return opt;
    }

    if (spj_pfct("[status]", msg))
    {
      return status_msgs();
    }

    if (get_st(msg, ".resend") == 0 && (CHECK))
    {
      char tmp[1000];
      memset(tmp, 0, sizeof tmp);
      get_copy(7, strlen(msg), msg, tmp);
      // tmp[strlen(tmp)]=0
      strchg("&#91;", "[", tmp);
      strchg("&#93;", "]", tmp);
      strchg("&amp;", "&", tmp);
      // puts(tmp);
      return string("{%") + tmp + "}";
    }
    if (get_st(msg, ".repost") == 0 && (CHECK))
    {
      char tmp[1000];
      memset(tmp, 0, sizeof tmp);
      get_copy(7, strlen(msg), msg, tmp);
      // tmp[strlen(tmp)]=0
      strchg("&#91;", "[", tmp);
      strchg("&#93;", "]", tmp);
      strchg("&amp;", "&", tmp);
      // puts(tmp);
      return tmp;
    }
    if (get_st(msg, ".cmd") == 0 && (CHECK))
    {
      return run_cmd(type);
    }
    if (spj_rand(msg))
    {
      char *temp2 = new char[10];
      get_copy(3, strlen(msg), msg, temp2);
      int rr;
      sscanf(temp2, "%d", &rr);
      memset(temp2, 0, sizeof temp2);
      if (rr == 0)
      {
        return "[Divide0]";
      }
      sprintf(temp2, "%d", rand() % rr);
      return temp2;
    }
  }
  

  return "[SPJ_CONTINUE]";
}

string run_cmd(Msg_type type)
{
  color_lable("[run_cmd]");
  char tmp[1000] = {0};
  char buff[1 << 16] = {0};
  int l, r;
  string msg = type.msgs;
  color_puts(msg.c_str());
  l = get_st(msg.c_str(), "#");
  r = strlen(msg.c_str());
  get_copy(l + 1, r, msg.c_str(), tmp);
  strchg("&#91;", "[", tmp);
  strchg("&#93;", "]", tmp);
  strchg("&amp;", "&", tmp);
#ifndef LINUX_V
  string ttp = UTF8ToGBK(tmp);
#else
  string ttp = tmp;
#endif
  // color_puts(ttp.c_str());
  execmd(ttp.c_str(), buff);
  // color_puts(buff);
  // output(type,buff);
  return buff;
}

#define MOE_READ(FILENAME, NAME, NAME_NUM) \
  {                                        \
    freopen(FILENAME, "r", stdin);         \
    char tmp[100];                         \
    tmp[0] = 0;                            \
    gets(tmp);                             \
    while (tmp[0] != '/')                  \
    {                                      \
      sscanf(tmp, "%s", NAME[NAME_NUM++]); \
      gets(tmp);                           \
    }                                      \
  }
string rand_mbti()
{
  char ti[5];
  ti[0] = rand() & 1 ? 'I' : 'E';
  ti[1] = rand() & 1 ? 'N' : 'S';
  ti[2] = rand() & 1 ? 'F' : 'T';
  ti[3] = rand() & 1 ? 'P' : 'J';
  ti[4] = 0;
  string rt(ti);
  return rt;
}
string lpscq(const Msg_type &type)
{
  // printf("调用lpscq\n");
  char optmsg[2000];
  char zokusai[200][20], kami[50][15], kamiclr[50][15], eye[50][15], eyeclr[50][15];
  int zokusai_num = 0, kami_num = 0, kamiclr_num = 0, eye_num = 0, eyeclr_num = 0;
  int rrd = rand() % 100;
  memset(optmsg, 0, sizeof optmsg);
  MOE_READ("moe_zokusai.txt", zokusai, zokusai_num)
  MOE_READ("moe_kami.txt", kami, kami_num)
  MOE_READ("moe_kamiclr.txt", kamiclr, kamiclr_num)
  MOE_READ("moe_eye.txt", eye, eye_num)
  MOE_READ("moe_eyeclr.txt", eyeclr, eyeclr_num)
  sprintf(optmsg, "又来啦？少yy些奇怪的东西吧！[\\n][name]YY出的老婆：[\\n]");
  string mbti = rand_mbti();
  int ht, wt, rsttime = 0;
rst:
  ht = 110 + rand() % 81;
  wt = 20 + rand() % 60;
  double bmi = wt / (ht * ht / 10000.0);
  if (bmi > 24.5 || bmi < 16)
  {
    if (rand() % 100 > 5)
    {
      rsttime++;
      goto rst;
    }
  }
  int midnum[5];
  if (rand() % 100 < 3)
  {
  bom:
    sprintf(optmsg + strlen(optmsg), "[\\n]……哎呀，[name]的老婆爆炸了！");
    goto moe_opt;
  }
  sprintf(optmsg + strlen(optmsg), "【性别】：");
  if (rrd < 3)
    sprintf(optmsg + strlen(optmsg), "男");
  else if (rrd < 7)
    sprintf(optmsg + strlen(optmsg), "伪郎");
  else if (rrd < 15)
    sprintf(optmsg + strlen(optmsg), "伪娘");
  else if (rrd < 18)
    sprintf(optmsg + strlen(optmsg), "无性别");
  else if (rrd < 20)
    sprintf(optmsg + strlen(optmsg), "扶她");
  else if (rrd < 100)
    sprintf(optmsg + strlen(optmsg), "女");
  sprintf(optmsg + strlen(optmsg), "[\\n]【身高】：%dcm[\\n]【体重】：%dkg[\\n]", ht, wt);
  sprintf(optmsg + strlen(optmsg), "【CUP】：");
  rrd = rand() % 6;
  if (rrd == 0)
    sprintf(optmsg + strlen(optmsg), "A-[\\n]");
  else
    sprintf(optmsg + strlen(optmsg), "%c[\\n]", 'A' + rrd - 1);
  if (rrd == 5)
  {
    sprintf(optmsg + strlen(optmsg), "[\\n]渡劫判定！！");
    midnum[0] = rand() % 20;
    midnum[1] = rand() % 100;
    sprintf(optmsg + strlen(optmsg), "[\\n]神秘力量的威力值为%d！！[\\n][name]的老婆的抵抗数值为%d！！[\\n]", midnum[1], midnum[0]);
    if (midnum[0] > midnum[1])
      sprintf(optmsg + strlen(optmsg), "[\\n]渡⭐劫⭐大⭐成⭐功[\\n]");
    else
    {
      sprintf(optmsg + strlen(optmsg), "[\\n]渡⭐劫⭐大⭐失⭐败[\\n]");
      goto bom;
    }
  }
  sprintf(optmsg + strlen(optmsg), "【发型】：%s[\\n]", kami[rand() % kami_num]);
  sprintf(optmsg + strlen(optmsg), "【发色】：%s[\\n]", kamiclr[rand() % kamiclr_num]);
  sprintf(optmsg + strlen(optmsg), "【眼型】：%s[\\n]", eye[rand() % eye_num]);
  sprintf(optmsg + strlen(optmsg), "【眼色】：%s[\\n]", eyeclr[rand() % eyeclr_num]);
  sprintf(optmsg + strlen(optmsg), "【萌点】：%s[\\n]", zokusai[rand() % zokusai_num]);
  sprintf(optmsg + strlen(optmsg), "【MBTI】：%s", mbti.c_str());
  sprintf(optmsg + strlen(optmsg), "[\\n][name]，你知道吗，你的老婆在诞生过程中经历了%d次重开哦", rsttime);
moe_opt: // output(type,optmsg);
  return optmsg;
}

/**********************************************************/
#include <map>
map<string, string> chems;
bool if_Chem_read;
void read_chem()
{
  freopen("chem.lst", "r", stdin);
  char tmp1[50] = {0}, tmp2[50] = {0};
  string a, b;
  char tmptxt[50];
  gets(tmptxt);
  while (tmptxt[0] != '/')
  {
    sscanf(tmptxt, "%s %s", &tmp1, &tmp2);
    // printf("%s %s\n",tmp1,tmp2);
    a = tmp1;
    b = tmptxt;
    chems[a] = b;
    gets(tmptxt);
  }
  if_Chem_read = 1;
  return;
}
string Chem(Msg_type type)
{
  if (!if_Chem_read)
    read_chem();
  string msg = type.msgs;
  char tmp[1000] = {0};
  int l, r;
  l = get_st(msg.c_str(), "#");
  r = strlen(msg.c_str());
  get_copy(l + 1, r, msg.c_str(), tmp);
  string p = tmp;
  if (chems[p].size() == 0)
  {
    return "无该元素信息";
  }
  p = chems[p];
  return p;
}

#endif
