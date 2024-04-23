#ifndef JIEMENG_SOCKET
#define JIEMENG_SOCKET
#include <string>
#include <mutex>
#include <condition_variable>
#include <nlohmann/json.hpp>
#include "Jiemeng_Exception.hpp"
using string = std::string;
using json = nlohmann::json;

class WSIO_Cache;
class Server
{
private:
  WSIO_Cache *wsio_cache;
  unsigned short ids=0;
  WSIO_Cache *get_cache();
  bool flag=0;
  //void flag_cache_true();
  //void flag_cache_false();
  
  mutex mtx;
  //std::unique_lock<std::mutex> lock;
public:
  string host;
  string port;

  condition_variable cv;
  void init(const string &h, const string &p);
  void run(std::function<void(const json&)>);
  json ws_send(json &);
  json get_message();
};

#endif
