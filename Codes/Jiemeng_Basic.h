#ifndef JIEMENG_BASIC
  #define JIEMENG_BASIC
  #define CURRENT_VERSION ("Jiemeng v20.7.0")
  #define UPDATE_TIME ("2023-02-03 10:19")
  #include<string.h>
  #include<windows.h>
  #include<stdio.h>
  #include"opstring.h"
  #include<stdlib.h>
  #include<direct.h>
  #include<unistd.h>
  #include<time.h>

  inline void print_time_mark(){
    int hr,mn,sec;
    time_t tmpcal_ptr;
  	struct tm *tmp_ptr = NULL;
  	time(&tmpcal_ptr);
  	tmp_ptr = localtime(&tmpcal_ptr);
    hr=tmp_ptr->tm_hour;
    mn=tmp_ptr->tm_min;
    sec=tmp_ptr->tm_sec;
    printf("\033[0m\033[0;37m[%02d:%02d:%02d]",hr,mn,sec);
  }
  inline void msg_lable(const char*s){
    print_time_mark();
    printf("\033[0m\033[1;7;36m%s\033[0m",s);
  }inline void msg_print(const char*s){
    printf("\033[0m\033[1;36m%s\033[0m",s);
  }inline void msg_puts(const char*s){
    printf("\033[0m\033[1;36m%s\n\033[0m",s);
  }inline void debug_print(const char*s){
    printf("\033[0m\033[7;32m%s\033[0m",s);
  }inline void debug_puts(const char*s){
    printf("\033[0m\033[7;32m%s\n\033[0m",s);
  }inline void info_lable(const char*s){
    print_time_mark();
    printf("\033[0m\033[1;7;32m%s\033[0m",s);
  }inline void info_print(const char*s){
    printf("\033[0m\033[1;32m%s\033[0m",s);
  }inline void info_puts(const char*s){
    printf("\033[0m\033[1;32m%s\n\033[0m",s);
  }inline void color_lable(const char*s){
    print_time_mark();
    printf("\033[0m\033[1;7;34m%s\033[0m",s);
  }inline void color_puts(const char*s){
    printf("\033[0m\033[1;34m%s\n\033[0m",s);
  }inline void color_print(const char*s){
    printf("\033[0m\033[1;34m%s\033[0m",s);
  }inline void error_lable(const char*s){
    print_time_mark();
    printf("\033[0m\033[5;7;31m%s\033[0m",s);
  }inline void error_puts(const char*s){
    printf("\033[0m\033[5;31m%s\n\033[0m",s);
  }inline void error_print(const char*s){
    printf("\033[0m\033[5;31m%s\033[0m",s);
  }
  bool execmd(const char*cmd,char*result) {
    char buffer[128];
    FILE*pipe=popen(cmd,"r");
    if(!pipe)return 0;
    int cnt=0;
    while(!feof(pipe)&&cnt<1000){
      if(fgets(buffer,128,pipe)){
        strcat(result,buffer);
        cnt++;
      }
    }
    _pclose(pipe);
    return 1;
  }


#endif
