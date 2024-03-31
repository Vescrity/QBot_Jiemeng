#ifndef JIEMENG_SOCKET
#define JIEMENG_SOCKET
#include <string>

#include <nlohmann/json.hpp>
#include "Jiemeng_Exception.hpp"
using string = std::string;
using json = nlohmann::json;
/// @brief Process message and creat a task thread to handle it.
void ProcessMessage(const std::string &message);

class WSIO_Cache;
class Server
{
private:
  WSIO_Cache *wsio_cache;
  unsigned short ids;

public:
  string host;
  string port;
  void init(const string &h, const string &p);
  void run();
  json ws_send(json &);
  json get_message();
};

#endif
