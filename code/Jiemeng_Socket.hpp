#ifndef JIEMENG_SOCKET
#define JIEMENG_SOCKET
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_Server.hpp"
#include <condition_variable>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
using string = std::string;
using json = nlohmann::json;
using Jiemeng::Server;
namespace Jiemeng {
class WSIO_Cache;
class WS_Backend : public Server {
  private:
    WSIO_Cache *wsio_cache;
    unsigned short ids = 0;
    bool flag = 0;
    // void flag_cache_true();
    // void flag_cache_false();

    mutex mtx;
    json ws_send(json &);
    // std::unique_lock<std::mutex> lock;
  public:
    string host;
    int port;
    bool enable_send;
    void run(std::function<void(const json &)> func) override;
    condition_variable cv;
    void init(const string &h, const int &p);
};

} // namespace Jiemeng

#endif
