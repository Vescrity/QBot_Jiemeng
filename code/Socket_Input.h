#ifndef SOCKET_INPUT
  #define SOCKET_INPUT
  #ifndef LINUX_V
  #include<winsock2.h>
  #else
  #include<sys/socket.h>
  #include<netinet/in.h>
  #include<arpa/inet.h>
  #define SOCKADDR sockaddr
  #define SOCKET int
  #endif
#include"Msg_type.h"
#include"Special_Judge.h"
#include<string>
#include<string.h>
#ifndef LINUX_V
WSADATA wsaData;
SOCKET servSock;
#else
int servSock;
#endif

sockaddr_in sockAddr;
#define BUF_SIZE (1<<12)
bool ifwhite(Msg_type type){

  if(!Bot_Judge.judge_not_bot(type.msg_id))return 0;
  if(type.ord_lv>99)return 1;
  string tmp(type.sender_id);

  for(int i=0;i<black_list_num;i++){
    if(tmp==black_list[i])return 0;
  }


  tmp=string(type.grp_id);
  if(type.ifgrp){
    for(int i=0;i<white_list_num;i++){
      if(tmp==Grp_white_list[i])return 1;
    }return 0;
  }

  if(CHECK)return 1;

  tmp=type.sender_id;
  for(int i=0;i<Pri_white_list_num;i++){
    if(tmp==Pri_white_list[i]){
      return Pri_blackorwhite;
    }
  }return Pri_blackorwhite?0:1;
}
  int wsainit(){
    #ifndef LINUX_V
    WSAStartup( MAKEWORD(2, 2), &wsaData);
    #endif
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址


    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址


    sockAddr.sin_port = htons(INPORT);  //端口
    return bind(servSock, (SOCKADDR*)&sockAddr,sizeof(SOCKADDR));
  }
  Msg_type init(){

      ss:
      if(listen(servSock,SOMAXCONN)!=0){
        #ifndef LINUX_V
        closesocket(servSock);
        #else
        close(servSock);
        #endif

        #ifndef LINUX_V
        ::WSACleanup();
        #endif
        error_lable("[init]");
        error_puts("listen to servsock失败，重试中...");
        wsainit();
        uni_sl(sleep_time);
        goto ss;
      }
      SOCKADDR clntAddr;
      #ifndef LINUX_V
      int nSize=sizeof(SOCKADDR);
      #else
      socklen_t nSize=sizeof(SOCKADDR);
      #endif
      SOCKET clntSock=accept(servSock,(SOCKADDR*)&clntAddr,&nSize);
      char buffer[BUF_SIZE];  //缓冲区
      memset(buffer,0,sizeof buffer);
      int strLen=recv(clntSock,buffer,BUF_SIZE,0);
      //printf("%d\n",strLen);
      #ifndef LINUX_V
      closesocket(clntSock);
      #else
      close(clntSock);
      #endif
      //closesocket(servSock);
      //::WSACleanup();
      //reward();
      if(strLen>((1<<12)-8)){uni_sl(sleep_time);goto ss;}
      Msg_type type,emp;
      emp.if_ignore=1;

      if(strlen(buffer)==0){color_lable("[init]");color_puts("len=0!");return emp;}
      type.get_msg_type(buffer);
      //type.show();
      if((!ifwhite(type))){color_lable("[init]");color_puts("not white!");return emp;}
      /*if(spj(type,buffer,type.msgs.c_str())){
        return emp;
      }*/
      stats_refresh();
      return type;

  }
#endif
