#ifndef SOCKETS_IO
#define SOCKETS_IO
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <thread>
#include <nlohmann/json.hpp>
#include "Jiemeng_Exception.h"
#include "CQjson.h"
#include "Jiemeng_Config_Class.h"

using boost::asio::ip::tcp;
using namespace boost::asio;
using tcp = boost::asio::ip::tcp;

namespace websocket = boost::beast::websocket;
static std::string serverHost;
static std::string serverPort;

void Main_Task(const json &);

/// @brief Process message and creat a task thread to handle it.
void ProcessMessage(const std::string &message)
{
  // 在这里处理接收到的消息
  json ev = json::parse(message);
  // raw_generate(ev);
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
    JM_EXCEPTION("[Socket_Client]")
  }
}

json ws_json_send(json &js)
{
  boost::asio::io_context io_send;
  debug_puts(js.dump().c_str());

  // 创建解析器和WebSocket对象
  tcp::resolver send_res(io_send);
  websocket::stream<tcp::socket> ws_send(io_send);
  boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> send_server_result;
  send_server_result = send_res.resolve(serverHost, serverPort);
  boost::asio::connect(ws_send.next_layer(), send_server_result.begin(), send_server_result.end());

  // WebSocket握手
  ws_send.handshake(serverHost, "/api");
  static short ids = 0;
  js["echo"] = ids;
  json rt;

  ws_send.write(boost::asio::buffer(js.dump()));
  int x = 0;
  // 接收服务器的响应
  do
  {
    boost::beast::flat_buffer buffer;
    ws_send.read(buffer);
    std::string response(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
    rt = json::parse(response);
    if (!rt["echo"].is_null())
    {
      if (rt["echo"] == ids)
        break;
    }
  } while ((x++) < 15);
  // 将响应解析为JSON对象并返回
  if (ids++ == 128)
    ids = 0;
  ws_send.close(websocket::close_code::normal);
  return rt;
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
