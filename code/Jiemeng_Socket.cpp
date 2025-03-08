#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Basic.h"
#include "Jiemeng_LogIO.hpp"
#include "Jiemeng_String.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;
using namespace boost::asio;
using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;
namespace net = boost::asio;
namespace websocket = boost::beast::websocket;
static const unsigned short CACHE_LENGTH = 0xffff;
/// @brief Web_Socket Control Center
namespace Jiemeng {
class WSIO_Cache {
  private:
    mutex mtx[CACHE_LENGTH];
    condition_variable cv[CACHE_LENGTH];
    bool flag[CACHE_LENGTH];
    json cache[CACHE_LENGTH];
    mutex msg_mtx;
    condition_variable msg_cv;
    bool msg_flag;
    net::io_context ioContext;
    std::string serverHost;
    int serverPort;
    boost::beast::multi_buffer buffer;
    tcp::resolver resolver;

  public:
    bool enable_send;
    websocket::stream<tcp::socket> ws;
    WSIO_Cache(const std::string &host, const int &port)
        : serverHost(host), serverPort(port), resolver(ioContext),
          ws(ioContext) {
        auto const results = resolver.resolve(serverHost, num2str(serverPort));
        msg_flag = 0;
        memset(flag, 0, sizeof flag);
        boost::asio::connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake(serverHost, "/");
    }
    void listen(std::function<void(const json &)> func);
    json read(const int &index);
};
void WSIO_Cache::listen(std::function<void(const json &)> func) {
    try {
        while (1) {
            boost::beast::flat_buffer buffer;
            ws.read(buffer);
            std::string message(boost::asio::buffers_begin(buffer.data()),
                                boost::asio::buffers_end(buffer.data()));
            buffer.consume(buffer.size());
            json recv;
            try {
                recv = json::parse(message);
            } catch (exception &e) {
                JM_EXCEPTION("[WSIO_Cache]");
                continue;
            }
            if (recv.contains("echo")) {
                if (enable_send) {
                    int index = recv["echo"].get<int>();
                    std::unique_lock<std::mutex> lock(mtx[index]);
                    cache[index] = recv; // 存入缓存数组
                    flag[index] = true;  // 设置标志位为可访问
                    lock.unlock();
                    cv[index].notify_one(); // 通知条件变量
                }
            } else {
                std::thread([this, func, recv] { func(recv); }).detach();
                debug_lable("[WSIO]");
                debug_puts("Detach!");
            }
        }
    } catch (exception &e) {
        JM_EXCEPTION("[WSIO_Cache]");
        throw e;
    }
}
json WSIO_Cache::read(const int &index) {
    if (index > CACHE_LENGTH)
        throw range_error("Out of range.");
    std::unique_lock<std::mutex> lock(mtx[index]);
    cv[index].wait(
        lock, [&] { return flag[index]; }); // 等待条件变量，直到标志位为可访问
    json result = cache[index];             // 获取JSON内容
    flag[index] = false;                    // 访问结束，标志位设为不可访问
    lock.unlock();                          // 解锁互斥量
    return result;
}

void WS_Backend::init(const string &h, const int &p) {
    host = h;
    port = p;
    enable_send = false;
}
void WS_Backend::run(std::function<void(const json &)> func) {
    while (1) {
        try {
            debug_lable("[Server]");
            debug_puts("Server::run 开始一次循环");
            wsio_cache = new WSIO_Cache(host, port);
            wsio_cache->enable_send = enable_send;
            try {
                wsio_cache->listen(func);
                debug_lable("[Server]");
                debug_puts("Server::run 结束listen");
            } catch (std::exception &e) {
                JM_EXCEPTION("[Server]")
                debug_puts("flag0");
                delete wsio_cache;
                debug_puts("flag1");
            }
        }

        catch (std::exception &e) {
            JM_EXCEPTION("[Server]")
            debug_puts("flag2");
            minisleep(5000);
        }
        debug_lable("[Server]");
        debug_puts("Server::run 完成一次循环");
    }
}

json WS_Backend::ws_send(json &js) {
    if (enable_send) {
        js["echo"] = ids++;
        if (ids == CACHE_LENGTH)
            ids = 0;
        debug_lable("[ws_send]");
        debug_puts(js.dump().c_str());
        json rt;
        wsio_cache->ws.write(boost::asio::buffer(js.dump()));
        rt = wsio_cache->read(js["echo"]);
        return rt;
    } else
        throw std::runtime_error(
            "ws_send is disabled, if you see this then there's a bug");
}
} // namespace Jiemeng
