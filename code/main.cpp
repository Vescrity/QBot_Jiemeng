#include "Jiemeng.hpp"
bool Debug_Mode=1;
int main()
{
  work_dir_check();
  Jiemeng bot;
  bot.init();
  bot.run();
}