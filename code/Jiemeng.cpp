
#include "Jiemeng.hpp"
void Heart_Beat()
{
  /*for (;;)
  {
    int tn = Main_thr_num.get_new_num();
#ifdef _WIN32
    Main_thread_list[tn] = thread(WinExec, configs.SYMBOL_NAME.c_str(), SW_HIDE);
#else
    Main_thread_list[tn] = thread(system, string("./" + configs.SYMBOL_NAME).c_str());
#endif
    Main_thread_list[tn].detach();
    minisleep(10000);
  }*/
}

int main()
{
  int st = clock();

    // TODO: WORKDIR CHECK:
#ifndef _WIN32
  execmd("mkdir tmp"s);
#endif
#ifdef _WIN32
  system("chcp 65001");
#endif
  Config_File_Read();
#ifdef _WIN32
  system(("title "s + configs.TITLE).c_str());
#endif

  Ans_Read();
  Pre_Catch_Init();
  int thrnum = Main_thr_num.get_new_num();
#ifndef NO_HB
  /*Main_thread_list[thrnum] = thread(Heart_Beat);
  Main_thread_list[thrnum].detach();
  thrnum = Main_thr_num.get_new_num();*/
#endif
#ifdef JIEMENG_DECK
  decks.init();
#endif
  Main_thread_list[thrnum] = thread(Time_Main);
  Main_thread_list[thrnum].detach();
  info_lable("[Start]");
  char *buf = new char[1 << 10];
  sprintf(buf, "启动成功，耗时%.3lfms", (clock() - st) * 1000.0 / CLOCKS_PER_SEC);
  info_puts(buf);
  delete[] buf;
  start_server(configs.PORT);
}
void Main_Task(const json &event)
{
  Message_Type *type = new Message_Type;
  type->init(event);
  if (type->btype.message == "[INVALID EVENT]"s)
    return;
  if (type->white)
    if (type->btype.is_white())
      Message_Operate(*type);

  delete type;
}
