#ifndef JIEMENG_OPERATION
#define JIEMENG_OPERATION
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
using string = std::string;
namespace Jiemeng {
class Message;
class Bot;
class Operation {
  public:
    enum class Type {
        message,
        order, // json order
        lua_call,
        lua_shell,
        lua_once,
        call_state,
        ignore,
        sleep,
        draw_deck,
        clear
    } type;
    bool is_json;
    json data;
    string str;
    Operation();
    void set_type(const string &);
    class Clear : std::exception {};
    class reRecv : public std::exception {
        std::string message;

      public:
        reRecv(const std::string &msg) : message(msg) {}
        virtual const char *what() const noexcept override {
            return message.c_str();
        }
    };
};

class Operation_List {

  public:
    std::vector<Operation> list;
    void upgrade(const Message &, Bot *);
    void push_back(const Operation &b) { *this += b; };
    void clear_ignore();
    Operation_List &operator+=(const Operation_List &b) {
        list.insert(list.end(), b.list.begin(), b.list.end());
        return *this;
    }
    Operation_List &operator+=(const Operation &b) {
        list.push_back(b);
        return *this;
    }

    Operation_List operator+(const Operation_List &b) const {
        Operation_List result = *this;
        result += b;
        return result;
    }
    Operation_List operator+(const Operation &b) const {
        Operation_List result = *this;
        result += b;
        return result;
    }
};
} // namespace Jiemeng

#endif
