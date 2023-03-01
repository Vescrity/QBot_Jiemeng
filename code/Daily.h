#ifndef DAILY
  #define DAILY
  #include<stdio.h>
  #include<time.h>
  #include<stdlib.h>
  #include"Jie_Meng_Base.h"
  static int last_tm=2500;
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
    type.sender_id="ADMIN";
    int ctm=hr*100+mn;
    if(ctm!=last_tm){
      char ss[10]={0};
      sprintf(ss,"[%02d:%02d]",hr,mn);
      resend(type,ss);
      last_tm=ctm;
    }

  }
  void Daily(){
    for(;;){
      Daily_check();
      uni_sl(30000);
    }
  }
#endif
