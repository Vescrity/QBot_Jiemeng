#include "Jiemeng_Http.hpp"
#include "Jiemeng_Config.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_IO.hpp"
#include "Jiemeng_Request.hpp"
#include "Jiemeng_Server.hpp"
#include "Jiemeng_String.hpp"
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <string>
using boost::asio::ip::tcp;
using json = nlohmann::json;
void handle_request(const std::string &request, tcp::socket &socket,
                    std::function<void(const json &)> func) {
    // 处理请求并回复响应

    /*int l = get_st(request.c_str(), "\r\n\r\n");
    std::thread t(Main_Task, string(request.c_str() + l + 4));
    t.detach();*/
}

namespace Jiemeng {
json Server::onebot_api(const string &api, json &data) {
    Request rq;
    rq.set_url("127.0.0.1:"s + std::to_string(sendport));
    rq.set_api("/"s + api);
    rq.set_data(data);
    return rq.js_post();
}
void Server::set_sendport(const int &p) {
    sendport = p;
}
void HTTP_Backend::run(std::function<void(const json &)> func) {

    auto listener = [&](tcp::socket &socket) {
        try {
            debug_lable("[Listen]");
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
            auto handler = [&] {
                std::string response =
                    "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n\r\nHello!";
                boost::asio::write(socket, boost::asio::buffer(response));
                debug_puts(request);
                // TODO: Run func
                // std::thread(func).detach();
            };
            handler();
            // handle_request(request, socket);
        } catch (std::exception &e) { JM_EXCEPTION("[HttpListen]") }
    };
    auto start = [&] {
        try {
            // TODO: Not sure for just replace io_service to context
            boost::asio::io_context io_context;

            tcp::acceptor acceptor(io_context,
                                   tcp::endpoint(tcp::v4(), inport));
            info_lable("[Server]");
            info_puts("Jiemeng sever starting...");
            while (true) {
                debug_lable("[Listen1]");
                tcp::socket socket(io_context);
                debug_lable("[Listen2]");
                acceptor.accept(socket);
                debug_lable("[Listen3]");
                listener(socket);
                socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                minisleep(500);
            }
        } catch (std::exception &e) {
            JM_EXCEPTION("[HttpStarter]")
            minisleep(5000);
        }
    };
    start();
}
} // namespace Jiemeng
