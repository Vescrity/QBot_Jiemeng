#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include "Jiemeng_Basic.h"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_DebugIO.hpp"

using boost::asio::ip::tcp;
using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;
namespace net = boost::asio;
namespace websocket = boost::beast::websocket;

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
  void listen();
  json read(const int &index);
};

void WSIO_Cache::listen()
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
json WSIO_Cache::read(const int &index)
{
  if (index > 129)
    throw range_error("Out of range.");
  std::unique_lock<std::mutex> lock(mtx[index]);
  cv[index].wait(lock, [&]
                 { return flag[index]; }); // 等待条件变量，直到标志位为可访问
  json result = cache[index];              // 获取JSON内容
  flag[index] = false;                     // 访问结束，标志位设为不可访问
  lock.unlock();                           // 解锁互斥量
  return result;
}
/*
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
}*/

//-----Server------

void Server::run()
{
  while (1)
  {
    try
    {
      wsio_cache = new WSIO_Cache(host, port);
      wsio_cache->listen();
    }
    catch (std::exception &e)
    {
      delete wsio_cache;
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

json Server::ws_send(json &js)
{
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
