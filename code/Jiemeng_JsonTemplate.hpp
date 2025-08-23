#ifndef JIEMENG_JSONTEMPLATE
#define JIEMENG_JSONTEMPLATE

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using string = std::string;
/// @brief Try to extract the value from a JSON by the given key and store it in
/// the given var
/// @tparam T The type of the given var
/// @param a The var to store the value
/// @param js The JSON contain the value
/// @param str The key to extract the value for
/// @throws std::invalid_argument if the given key cannot found in the given
/// JSON
template <typename T> void js_getval(T &a, const json &js, const string &str) {
    if (!js.contains(str))
        throw std::invalid_argument(str + " is not given (config)");

    else
        a = js[str];
}
/// @brief Extract the value from a JSON by the given key and store it in the
/// given var
/// @tparam T The type of the given var
/// @param a The var to store the value
/// @param js The JSON contain the value
/// @param str The key to extract the value for
/// @param b If the given key cannot be found in the given JSON then set `a = b`
template <typename T>
void js_getval(T &a, const json &js, const string &str, const T &b) {
    if (!js.contains(str))
        a = b;
    else {
        try {
            a = js[str];
        } catch (const std::exception &e) {
            a = b;
            throw std::invalid_argument(string(e.what()) +
                                        "\n  at js_getval: str = " + str);
        }
    }
}

#endif
