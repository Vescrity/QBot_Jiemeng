#ifndef DAILY
#define DAILY
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Orders.h"
static int last_tm = 2500;
static void Daily_check()
{
  int hr, mn;
  time_t tmpcal_ptr;
  struct tm *tmp_ptr = NULL;
  time(&tmpcal_ptr);
  tmp_ptr = localtime(&tmpcal_ptr);
  hr = tmp_ptr->tm_hour;
  mn = tmp_ptr->tm_min;
  Basic_Message_Type type;
  type.admin_level = 100;
  type.ifgroup = 1;
  type.user_id = "ADMIN";

  int ctm = hr * 100 + mn;
  if (ctm != last_tm)
  {
    char ss[10] = {0};
    sprintf(ss, "[%02d:%02d]", hr, mn);
    type_resend(type, ss);
    last_tm = ctm;
  }
}
void Daily()
{
  for (;;)
  {
    Daily_check();
    minisleep(30000);
  }
}
#endif
