#ifndef JIEMENG_BASIC
  #define JIEMENG_BASIC
  #include<string.h>
  #include<windows.h>
  #include<stdio.h>
  #include"opstring.h"
  #include<stdlib.h>
  #include<direct.h>
  #include<unistd.h>
  bool execmd(const char* cmd,char* result) {
        char buffer[128];                                     //定义缓冲区
        FILE* pipe = _popen(cmd,"r");                  //打开管道，并执行命令
        if (!pipe)
            return 0;                                 //返回0表示运行失败
        int cnt=0;
        while(!feof(pipe)&&cnt<1000) {//10
        if(fgets(buffer,128,pipe)){                   //将管道输出到result中
                    strcat(result,buffer);
                    cnt++;
              }
        }
        _pclose(pipe);                                          //关闭管道
        return 1;                                                 //返回1表示运行成功
  }

#endif
