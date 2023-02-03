#include<winsock2.h>
#include<string>
#include<string.h>
#define INPORT 7860
#define BUF_SIZE (1000000)


int main(){

    ss:WSADATA wsaData;
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
    puts(buffer);
    goto ss;



}
