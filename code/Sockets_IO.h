#ifndef SOCKETS_IO
#define SOCKETS_IO
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include <nlohmann/json.hpp>
#include "Jiemeng_Exception.h"
#include "CQjson.h"
#include "Jiemeng_Config_Class.h"

using boost::asio::ip::tcp;
using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
namespace net = boost::asio;
namespace websocket = boost::beast::websocket;
static std::string serverHost;
static std::string serverPort;

void Main_Task(const json &);

/// @brief Process message and creat a task thread to handle it.
void ProcessMessage(const std::string &message)
{
  json ev = json::parse(message);
  // raw_generate(ev);
  std::thread t(Main_Task, ev);
  t.detach();
}
/// @brief Web_Socket Control Center
class WSIO_Cache
{
private:
  mutex mtx[130];
  condition_variable cv[130];
  bool flag[130];
  json cache[130];

  net::io_context ioContext;
  tcp::resolver resolver;

  std::string serverHost;
  std::string serverPort;
  boost::beast::multi_buffer buffer;

public:
  websocket::stream<tcp::socket> ws;
  WSIO_Cache(const std::string &host, const std::string &port)
      : resolver(ioContext), ws(ioContext), serverHost(host), serverPort(port)
  {
    auto const results = resolver.resolve(serverHost, serverPort);
    boost::asio::connect(ws.next_layer(), results.begin(), results.end());
    ws.handshake(serverHost, "/");
  }

public:
  void listen()
  {

    try
    {
      while (1)
      {
        boost::beast::flat_buffer buffer;
        ws.read(buffer);
        std::string message(boost::asio::buffers_begin(buffer.data()), boost::asio::buffers_end(buffer.data()));
        debug_lable("[WSIO_Recive]");
        dout << message << "\n";
        buffer.consume(buffer.size());
        json recv;
        try
        {
          recv = json::parse(message);
        }
        catch (exception &e)
        {
          JM_EXCEPTION("[WSIO_Cache]");
          continue;
        }
        if (recv.contains("echo"))
        {
          int index = recv["echo"].get<int>();
          std::unique_lock<std::mutex> lock(mtx[index]);
          cache[index] = recv; // 存入缓存数组
          flag[index] = true;  // 设置标志位为可访问
          lock.unlock();
          cv[index].notify_one(); // 通知条件变量
        }
        else
        {
          std::thread processThread(ProcessMessage, message);
          processThread.detach();
        }
      }
    }

    catch (exception &e)
    {
      JM_EXCEPTION("[WSIO_Cache]");
    }
  }
  json read(const int &index)
  {
    if (index > 129)
      throw range_error("Out of range.");
    std::unique_lock<std::mutex> lock(mtx[index]);
    cv[index].wait(lock, [&]
            { return flag[index]; }); // 等待条件变量，直到标志位为可访问
    json result = cache[index];       // 获取JSON内容
    flag[index] = false;              // 访问结束，标志位设为不可访问
    lock.unlock();                    // 解锁互斥量
    return result;
  }
};
WSIO_Cache *wsio_cache;

void WebSocketClient(const std::string &serverHost, const std::string &serverPort)
{
  wsio_cache = new WSIO_Cache(serverHost, serverPort);
  try
  {
    wsio_cache->listen();
  }
  catch (...)
  {
    delete wsio_cache;
  }
}

json ws_json_send(json &js)
{
  static short ids = 0;
  js["echo"] = ids++;
  if (ids == 128)
    ids = 0;
  debug_lable("[ws_send]");
  debug_puts(js.dump().c_str());
  json rt;
  wsio_cache->ws.write(boost::asio::buffer(js.dump()));
  rt = wsio_cache->read(js["echo"]);
  return rt;
}

void start_server(int port)
{

  while (1)
  {
    try
    {
      serverHost = "127.0.0.1";
      serverPort = to_string(port);
      WebSocketClient(serverHost, serverPort);
    }
    catch (std::exception &e)
    {
      std::string msg = "Exception caught: ";
      msg += e.what();
      error_lable("[Server]");
      error_puts(msg.c_str());
      error_lable("[Server]");
      error_puts("Backend is down. Try to rebind...");
      minisleep(5000);
    }
  }
}


#endif
