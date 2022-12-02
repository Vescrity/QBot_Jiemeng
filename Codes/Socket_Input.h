#ifndef SOCKET_INPUT
  #define SOCKET_INPUT
  #include<winsock2.h>
#include"Msg_type.h"
#include"Special_Judge.h"
#include<string>
#include<string.h>

#define BUF_SIZE (1<<15)
bool ifwhite(Msg_type type){
  string tmp(type.sender_id);
  //if(type.sender_id=="424492087")return 0;
  for(int i=0;i<black_list_num;i++){
    if(tmp==black_list[i])return 0;
  }tmp=string(type.grp_id);
  if(type.ifgrp){
    for(int i=0;i<white_list_num;i++){
      if(tmp==Grp_white_list[i])return 1;
    }return 0;
  }tmp=type.sender_id;
  for(int i=0;i<Pri_white_list_num;i++){
    if(tmp==Pri_white_list[i]){
      return Pri_blackorwhite;
    }
  }return Pri_blackorwhite?0:1;
}

  Msg_type init(char *msg){
    //try{
      ss:memset(msg,0,sizeof msg);
      WSADATA wsaData;
      WSAStartup( MAKEWORD(2, 2), &wsaData);
      SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
      sockaddr_in sockAddr;
      memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
      sockAddr.sin_family = PF_INET;  //使用IPv4地址
      sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
      sockAddr.sin_port = htons(INPORT);  //端口
      bind(servSock, (SOCKADDR*)&sockAddr,sizeof(SOCKADDR));
      listen(servSock, SOMAXCONN);
      SOCKADDR clntAddr;
      int nSize=sizeof(SOCKADDR);
      SOCKET clntSock=accept(servSock,(SOCKADDR*)&clntAddr,&nSize);
      char buffer[BUF_SIZE];  //缓冲区
      memset(buffer,0,sizeof buffer);
      int strLen=recv(clntSock,buffer,BUF_SIZE,0);

      /*char pot[10]={0};
      sprintf(pot,"%d",OUTPORT);
      std::string strSend = "HTTP/1.1 204 OK\r\n"
      "Host: 127.0.0.1:";
      strSend=strSend+pot+"\r\n"
      "Connection: close\r\n";
      send(clntSock, strSend.c_str(), strSend.length(), 0);*/

      closesocket(clntSock);
      closesocket(servSock);
      ::WSACleanup();
      //reward();
      //puts("socket");
      Msg_type type,emp;
      emp.grp_id[0]='-';
      if(strlen(buffer)==0)return emp;
      type.get_msg_type(buffer);
      get_msg(buffer,msg);
      Current_Msg=msg;
      if((!ifwhite(type)))return emp;
      if(spj(type,buffer,msg)){
        return emp;
      }

      stats_refresh();
      return type;
    //}
    /*catch(...){
      error_report(string("ERROR_in_SOCKET_INPUT::init"));
      Msg_type type;
      get_copy(0,8,"12345678",type.sender_id);
      type.ifgrp=0;
      return type;
    }*/
  }
#endif
