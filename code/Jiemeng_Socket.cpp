#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include "Jiemeng_Basic.h"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_LogIO.hpp"

using boost::asio::ip::tcp;
using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;
namespace net = boost::asio;
namespace websocket = boost::beast::websocket;
static const unsigned short CACHE_LENGTH = 64;
/// @brief Web_Socket Control Center
class WSIO_Cache
{
private:
  mutex mtx[CACHE_LENGTH];
  condition_variable cv[CACHE_LENGTH];
  bool flag[CACHE_LENGTH];
  json cache[CACHE_LENGTH];
  mutex msg_mtx;
  condition_variable msg_cv;
  json msg_cache;
  bool msg_flag;

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

  void listen();
  json read(const int &index);
  json get_message();
};

void WSIO_Cache::listen()
{
  try
  {
    while (1)
    {
      debug_lable("[Listen]");
      dout << "\n";
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
        // std::thread processThread(ProcessMessage, message);
        // processThread.detach();

        std::unique_lock<std::mutex> lock(msg_mtx);
        msg_cache = recv;
        msg_flag = true;
        lock.unlock();
        msg_cv.notify_one();
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
  if (index > CACHE_LENGTH)
    throw range_error("Out of range.");
  std::unique_lock<std::mutex> lock(mtx[index]);
  cv[index].wait(lock, [&]
                 { return flag[index]; }); // 等待条件变量，直到标志位为可访问
  json result = cache[index];              // 获取JSON内容
  flag[index] = false;                     // 访问结束，标志位设为不可访问
  lock.unlock();                           // 解锁互斥量
  return result;
}
json WSIO_Cache::get_message()
{
  std::unique_lock<std::mutex> lock(msg_mtx);
  msg_cv.wait(lock, [&]
              { return msg_flag; }); 
  json result = msg_cache;           
  msg_flag = false;                  
  lock.unlock();                     
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

void Server::init(const string &h, const string &p)
{
  ids = 0;
  host = h;
  port = p;
  wsio_cache = new WSIO_Cache(host, port);
}
void Server::run()
{
  while (1)
  {
    try
    {
      wsio_cache->listen();
    }
    catch (std::exception &e)
    {
      delete wsio_cache;
      wsio_cache = new WSIO_Cache(host, port);
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
  if (ids == CACHE_LENGTH)
    ids = 0;
  debug_lable("[ws_send]");
  debug_puts(js.dump().c_str());
  json rt;
  wsio_cache->ws.write(boost::asio::buffer(js.dump()));
  rt = wsio_cache->read(js["echo"]);
  return rt;
}
json Server::get_message() { return wsio_cache->get_message(); }
