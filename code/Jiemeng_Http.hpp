#pragma once
#include "Jiemeng_Server.hpp"
namespace Jiemeng {
class HTTP_Backend : public Server {
private:
    int inport;
public:
    void init(){inport=5703;};
    void run(std::function<void(const json &)> func) override;
    //json onebot_api(const string &api, json &data) ;
};
} // namespace Jiemeng
