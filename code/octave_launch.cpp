#include <thread>
#include "Jiemeng_Basic.h"
#include <iostream>
void funcs()
{
  cout << execmd("octave-cli tmp/mat_code.m");
  exit(0);
}
void timer()
{
  minisleep(1000);
  printf("TIME OUT");
  system("kill -9 $(pgrep -f mat_code)");
}

int main()
{
  thread a = thread(timer);
  a.detach();
  funcs();
  exit(1);
}
