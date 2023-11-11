#ifndef TXT2IMG_API
#define TXT2IMG_API
#include <string>
#include "Jiemeng_Basic.h"
using string = std::string;
string txt2img(const char *txt)
{
  const int N = 16;
  static int Now = 0;
  char name[16] = {0};
  char txtname[32] = {0};
  char filename[32] = {0};
  char excmd[128] = {0};
  sprintf(name, "text%03d", Now++);
  sprintf(txtname, "tmp/%s.txt", name);
  sprintf(filename, "tmp/%s.png", name);
  sprintf(excmd, "python txt2img/txt2img.py %s %s", txtname, filename);
  if (Now >= N)
    Now = 0;
  FILE *fp = fopen(txtname, "w");
  fprintf(fp, "%s", txt);
  fclose(fp);
  return execmd(excmd);
}
#endif