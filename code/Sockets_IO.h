#ifndef SOCKETS_IO
#define SOCKETS_IO
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <thread>
#include <nlohmann/json.hpp>
#include "CQjson.h"
#include "Jiemeng_Config_Class.h"
using boost::asio::ip::tcp;

using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

static std::string serverHost;
static std::string serverPort;

void Main_Task(const json &);

void ProcessMessage(const std::string &message)
{
  // 在这里处理接收到的消息
  std::cout << "Received message: " << message << std::endl;

  json ev = json::parse(message);
  raw_generate(ev);
  cout << "\nRAWOUT: " << ev << endl
       << endl;
  std::thread t(Main_Task, ev);
  t.detach();
}

void WebSocketClient(const std::string &serverHost, const std::string &serverPort)
{
  try
  {
    boost::asio::io_context ioContext;

    // 创建解析器和WebSocket对象
    tcp::resolver resolver(ioContext);
    websocket::stream<tcp::socket> ws(ioContext);

    // 解析服务器地址
    auto const results = resolver.resolve(serverHost, serverPort);

    // 连接到服务器
    boost::asio::connect(ws.next_layer(), results.begin(), results.end());

    // WebSocket握手
    ws.handshake(serverHost, "/event");

    // 接收和处理消息
    boost::beast::multi_buffer buffer;
    while (true)
    {
      ws.read(buffer);
      std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
      buffer.consume(buffer.size());

      // 在新线程中处理消息
      std::thread processThread(ProcessMessage, message);
      processThread.detach();
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
/*
void SendJsonMessage(const nlohmann::json &json, const std::string &serverHost, const std::string &serverPort)
{
  try
  {
    // 创建IO上下文
    boost::asio::io_context ioContext;

    // 创建解析器和WebSocket对象
    tcp::resolver resolver(ioContext);
    websocket::stream<tcp::socket> ws(ioContext);

    // 解析服务器地址
    auto const results = resolver.resolve(serverHost, serverPort);

    // 连接到服务器
    boost::asio::connect(ws.next_layer(), results.begin(), results.end());

    // WebSocket握手
    ws.handshake(serverHost, "/");

    // 发送JSON消息
    ws.write(boost::asio::buffer(json.dump()));

    // 关闭WebSocket连接
    ws.close(websocket::close_code::normal);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}*/
nlohmann::json SendJsonMessage(const nlohmann::json &json, const std::string &serverHost, const std::string &serverPort)
{
  try
  {
    // 创建IO上下文
    boost::asio::io_context ioContext;

    // 创建解析器和WebSocket对象
    tcp::resolver resolver(ioContext);
    websocket::stream<tcp::socket> ws(ioContext);

    // 解析服务器地址
    auto const results = resolver.resolve(serverHost, serverPort);

    // 连接到服务器
    boost::asio::connect(ws.next_layer(), results.begin(), results.end());

    // WebSocket握手
    ws.handshake(serverHost, "/api");

    // 发送JSON消息
    ws.write(boost::asio::buffer(json.dump()));

    // 接收服务器的响应
    boost::beast::flat_buffer buffer;
    ws.read(buffer);
    std::string response(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));

    // 关闭WebSocket连接
    ws.close(websocket::close_code::normal);

    // 将响应解析为JSON对象并返回
    return nlohmann::json::parse(response);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return nlohmann::json(); // 返回空的JSON对象表示出现异常
  }
}
json SendJsonMessage(const json &data)
{
  return SendJsonMessage(data, serverHost, serverPort);
}
void start_server(int port)
{
  try
  {
    while (1)
    {
      // 服务端的主机和端口
      serverHost = "127.0.0.1";
      serverPort = to_string(port);
      // 启动WebSocket客户端
      std::thread clientThread(WebSocketClient, serverHost, serverPort);
      info_lable("[Server]");
      info_puts("Jiemeng sever started.");
      clientThread.join();
      error_lable("[Server]");
      error_puts("Backend is down. Try to rebind...");
      minisleep(5000);
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
