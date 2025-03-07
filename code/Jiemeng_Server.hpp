#pragma once
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
using string = std::string;
using json = nlohmann::json;
namespace Jiemeng {
class Server {
    int sendport;

  public:
    void set_sendport(const int &p);
    virtual void run(std::function<void(const json &)> func) = 0;
    json onebot_api(const string &api, json &data);

    // TODO: 新的 Server 基类，以支持多通信后端
};

} // namespace Jiemeng
