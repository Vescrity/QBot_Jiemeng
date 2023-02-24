#ifndef HTTPPOST
#define HTTPPOST
#include<string>
#include<vector>
#include<iostream>
class HttpRequest{
  public:
    HttpRequest(const std::string& ip, int port);
    ~HttpRequest(void);
    std::string HttpGet(std::string req);
    std::string HttpPost(std::string req, std::string data);
    // 合成JSON字符串
    static std::string genJsonString(std::string key, std::string value);
    static std::string genJson(std::string key, std::string value);
    // 分割字符串
    static std::vector<std::string>split(const std::string&s,const std::string&seperator);
    // 根据key从Response获取Header中的内容
    static std::string getHeader(std::string respose,std::string key);
    std::string m_ip;
    int m_port;
};
#ifndef LINUX_V
#include <WinSock.h>
#endif
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
HttpRequest::HttpRequest(const std::string& ip, int port) : m_ip(ip), m_port(port){}
HttpRequest::~HttpRequest(void){}
// Http GET请求
std::string HttpRequest::HttpGet(std::string req)
{
  std::string ret = ""; // 返回Http Response
  try{
    WSADATA wData;
    ::WSAStartup(MAKEWORD(2, 2), &wData);
    SOCKET clientSocket = socket(AF_INET, 1, 0);
    struct sockaddr_in ServerAddr = {0};
    ServerAddr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    ServerAddr.sin_port = htons(m_port);
    ServerAddr.sin_family = AF_INET;
    int errNo = connect(clientSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if(errNo == 0)
    {
      std::string strSend = " HTTP/1.1\r\n"
            "Cookie:16888\r\n\r\n";
      strSend = "GET " + req + strSend;
      errNo = send(clientSocket, strSend.c_str(), strSend.length(), 0);
      if(errNo > 0){
            //cout << "发送成功" << endl;
      }
      else{
        std::cout << "errNo:" << errNo << std::endl;
        return ret;
      }
      char bufRecv[3069] = {0};
      errNo = recv(clientSocket, bufRecv, 3069, 0);
      if(errNo > 0){
        ret = bufRecv;// 如果接收成功，则返回接收的数据内容
      }
      else{
        std::cout << "errNo:" << errNo << std::endl;
        return ret;
      }
    }
    else{
      errNo = WSAGetLastError();
      std::cout << "errNo:" << errNo << std::endl;
    }
    // socket环境清理
    ::WSACleanup();

  }
  catch (...){
    return "";
  }
  return ret;
}

// Http POST请求
std::string HttpRequest::HttpPost(std::string req, std::string data)
{
    std::string ret = ""; // 返回Http Response
    try
    {
        // 开始进行socket初始化;
        WSADATA wData;
        ::WSAStartup(MAKEWORD(2, 2), &wData);

        SOCKET clientSocket = socket(AF_INET, 1, 0);
        struct sockaddr_in ServerAddr = {0};
        ServerAddr.sin_addr.s_addr = inet_addr(m_ip.c_str());
        ServerAddr.sin_port = htons(m_port);
        ServerAddr.sin_family = AF_INET;
        int errNo = connect(clientSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
        if(errNo == 0)
        {
            // 格式化data长度
            char len[10] = {0};
            sprintf(len, "%d", data.length());
            std::string strLen = len;
            char pot[10]={0};
            sprintf(pot,"%d",m_port);
            std::string strSend = " HTTP/1.1\r\nHOST: "+m_ip+":"+pot+"\r\nCookie:16888\r\n"
                //"Content-Type:application/x-www-form-urlencoded\r\n"
                "Content-Type:application/json\r\n"
                "Charset:utf-8\r\n"
                "Content-Length:";
            strSend = "POST " + req + strSend + strLen + "\r\n\r\n" + data;

            // 发送
            errNo = send(clientSocket, strSend.c_str(), strSend.length(), 0);
            if(errNo > 0)
            {
                //cout<<"发送成功\n";
            }
            else
            {
                std::cout << "errNo:" << errNo << std::endl;
                return ret;
            }

            // 接收
            char bufRecv[3069] = {0};
            errNo = recv(clientSocket, bufRecv, 3069, 0);
            if(errNo > 0)
            {
                ret = bufRecv;// 如果接收成功，则返回接收的数据内容
            }
            else
            {
                std::cout << "errNo:" << errNo << std::endl;
                return ret;
            }
        }
        else
        {
            errNo = WSAGetLastError();
        }
        // socket环境清理
        ::WSACleanup();
    }
    catch (...)
    {
        return "";
    }
    return ret;
}

// 合成JSON字符串
std::string HttpRequest::genJsonString(std::string key, std::string value)
{
    char buf[1<<15] = {0};
    sprintf(buf, "{\"%s\":\"%s\"}", key.c_str(), value.c_str());
    std::string ret = buf;
    return ret;
}
std::string HttpRequest::genJson(std::string key, std::string value)
{
    char buf[1<<15] = {0};
    sprintf(buf, "{\"%s\":%s}", key.c_str(), value.c_str());
    std::string ret = buf;
    return ret;
}

// 分割字符串
std::vector<std::string> HttpRequest::split(const std::string &s, const std::string &seperator)
{
    std::vector<std::string> result;
    typedef std::string::size_type string_size;
    string_size i = 0;

    while(i != s.size()){
        // 找到字符串中首个不等于分隔符的字母
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
        }

        // 找到又一个分隔符，将两个分隔符之间的字符串取出
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                }
                if(flag == 0)
                    ++j;
        }
        if(i != j){
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}

// 从Response中查找key对应的Header的内容
std::string HttpRequest::getHeader(std::string respose, std::string key)
{
    std::vector<std::string> lines = split(respose, "\r\n");
    for (int i = 0; i < lines.size(); i++)
    {
        std::vector<std::string> line = split(lines[i], ": ");// 注意空格
        if (line.size() >= 2 && line[0] == key)
        {
            return line[1];
        }
    }
    return "";
}

BOOL Get_IP(const char*szHost, char* szIp)
{
	WSADATA        wsaData;

	HOSTENT   *pHostEnt;
	int             nAdapter = 0;
	struct       sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		printf(" gethostbyname error for host:\n");
		return FALSE;
	}

	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		if (pHostEnt->h_addr_list[nAdapter]){
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);
			sprintf(szIp, "%s", inet_ntoa(sAddr.sin_addr));
		}
	}
	WSACleanup();
	return TRUE;
}
#endif
