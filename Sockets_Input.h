#ifndef SOCKETS_INPUT
#define SOCKETS_INPUT
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "Jiemeng_Config.h"
using boost::asio::ip::tcp;
void Main_Task(const std::string &);

void handle_request(const std::string &request, tcp::socket &socket)
{
  // 处理请求并回复响应
  std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n\r\nHello!";
  boost::asio::write(socket, boost::asio::buffer(response));

  int l = get_st(request.c_str(), "\r\n\r\n");
  std::thread t(Main_Task, string(request.c_str() + l + 4));
  t.detach();
}

void listen_for_connection(tcp::socket &socket)
{
  try
  {
    boost::system::error_code error;
    boost::asio::streambuf request_buf(1 << 16);

    // 读取请求体
    boost::asio::read_until(socket, request_buf, "}\n", error);
    if (error)
      throw boost::system::system_error(error);

    // 处理请求
    std::stringstream ss;
    ss << &request_buf;
    std::string request = ss.str();
    handle_request(request, socket);
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

void start_server(int port)
{
  try
  {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    info_lable("[Server]");
    info_puts("Jiemeng sever starting...");
    while (true)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);
      listen_for_connection(socket);
      socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      minisleep(configs.sleep_time);
    }
  }
  catch (std::exception &e)
  {
    std::string msg = "Exception caught: ";
    msg += e.what();
    error_lable("[Server]");
    error_puts(msg.c_str());
    minisleep(configs.sleep_time);
  }
}
#endif