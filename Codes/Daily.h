#ifndef DAILY
  #define DAILY
  #include<stdio.h>
  #include<time.h>
  #include<stdlib.h>
  #include<windows.h>
  #include"Jie_Meng_Base.h"
  void Daily_check(){
    int hr,mn;
    time_t tmpcal_ptr;
  	struct tm *tmp_ptr = NULL;
  	time(&tmpcal_ptr);
  	tmp_ptr = localtime(&tmpcal_ptr);
    hr=tmp_ptr->tm_hour;
    mn=tmp_ptr->tm_min;
    Msg_type type;
    type.ifgrp=1;
    get_copy(0,5,"ADMIN",type.sender_id);

    char ss[10]={0};
    sprintf(ss,"[%02d:%02d]",hr,mn);
    resend(type,ss);
  }
  void Daily(){
    for(;;){
      Daily_check();
      Sleep(59000);
    }
  }
#endif
