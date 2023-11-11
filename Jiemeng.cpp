
#include "Jiemeng.hpp"
void Heart_Beat()
{
  for (;;)
  {
    int tn = Main_thr_num.get_new_num();
#ifndef LINUX_V
    Main_thread_list[tn] = thread(WinExec, configs.SYMBOL_NAME.c_str(), SW_HIDE);
#else
    Main_thread_list[tn] = thread(system, string("./" + configs.SYMBOL_NAME).c_str());
#endif
    Main_thread_list[tn].detach();
    minisleep(10000);
  }
}

int main()
{
  int st = clock();
#ifndef LINUX_V
  system("chcp 65001");
#endif
  Config_File_Read();
#ifndef LINUX_V
  system(("title "s + configs.TITLE).c_str());
#endif

  Ans_Read();
  Pre_Catch_Init();
  int thrnum = Main_thr_num.get_new_num();
#ifndef NO_HB
  Main_thread_list[thrnum] = thread(Heart_Beat);
  Main_thread_list[thrnum].detach();
  thrnum = Main_thr_num.get_new_num();
#endif
  Main_thread_list[thrnum] = thread(Time_Main);
  Main_thread_list[thrnum].detach();
  info_lable("[Start]");
  char *buf = new char[1 << 10];
  sprintf(buf, "启动成功，耗时%dms", int((clock() - st) * 1000.0 / CLOCKS_PER_SEC));
  info_puts(buf);
  delete[] buf;
  start_server(configs.INPORT);
}
void Main_Task(const string &req)
{
  Message_Type *type = new Message_Type;
  type->init(req);
  if (type->white)
    if (type->btype.is_white())
      Message_Operate(*type);

  delete type;
}
