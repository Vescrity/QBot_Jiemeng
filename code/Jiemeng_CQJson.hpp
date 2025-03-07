#ifndef JIEMENG_CQJSON
#define JIEMENG_CQJSON

#include "Jiemeng_DebugIO.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using string = std::string;
namespace Jiemeng {

class CQMessage {
    mutable string cq;
    mutable json js;
    mutable bool json_ready = 0, string_ready = 0;
    void generate_json() const;
    void generate_string() const;

  public:
    CQMessage(const string &s) { change(s); }
    CQMessage(const json &j) { change(j); }
    json &get_json() const {
        if (!json_ready)
            generate_json();
        return js;
    }
    string &get_string() const {
        if (!string_ready)
            generate_string();
        return cq;
    }
    string &&str() {
        get_string();
        json_ready = 0;
        return std::move(cq);
    }
    string const_str() const { return get_string(); }
    string true_str() const;
    void change(const json &j) {
        js = j;
        json_ready = 1;
        string_ready = 0;
    }
    void change(const string &s) {
        cq = s;
        json_ready = 0;
        string_ready = 1;
    }
    void change(const char *s) { change(string(s)); }
    CQMessage &operator=(const string &s) {
        change(s);
        return *this;
    }
    // operator std::string() { return const_str(); }
};
} // namespace Jiemeng
#endif
