#ifndef SOCKET_INPUT
  #define SOCKET_INPUT
  /*读取上报消息并获取消息内容*/
#include"Msg_type.h"
#include"Special_Judge.h"
#include<string>
#include<string.h>
#include<winsock2.h>
#define BUF_SIZE 99999
bool ifwhite(Msg_type type){                        //判断是否白名单/不在黑名单
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
  Msg_type init(char *msg){                                                     //获取消息
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
    listen(servSock, 20);
    SOCKADDR clntAddr;
    int nSize=sizeof(SOCKADDR);
    SOCKET clntSock=accept(servSock,(SOCKADDR*)&clntAddr,&nSize);
    char buffer[BUF_SIZE];  //缓冲区
    memset(buffer,0,sizeof buffer);
    int strLen=recv(clntSock,buffer,BUF_SIZE,0);
    //puts(buffer);
    Msg_type type;
    type.get_msg_type(buffer);
    get_msg(buffer,msg);
    Current_Msg=msg;
    closesocket(clntSock);
    closesocket(servSock);
    /*if(!type.ifgrp){

      memset(msg,0,sizeof msg);
      get_copy(0,strlen(tmp.c_str()),tmp.c_str(),msg);
    }*/
    if((!ifwhite(type)))goto ss;
    if(spj(type,buffer,msg))
      goto ss;
    stats_refresh();
    return type;
  }
#endif
