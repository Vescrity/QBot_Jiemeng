#include "Jiemeng_Message.hpp"
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Config.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Version.hpp"
using namespace std;
namespace Jiemeng {

void Message_Place::get_level(const Config *config) {
    level = config->get_admin_level(user_id);
}
void Message::init(const json &js) {
    if (js["post_type"] != "notice" && js["post_type"] != "request")
        message_init(js);
    else
        notice_init(js);
}
void Message::message_init(const json &js) {
    message_id = to_string(js["message_id"]);
    if (js["message_type"] == "group")
        set_group();
    else
        set_private();
    change(js["message"]);
    const json &sender = js["sender"];
    user_id = to_string(js["user_id"]);
    user_nk = sender["nickname"];

    if (sender.contains("card"))
        if (sender["card"].is_string())
            user_nm = sender["card"];
    if (user_nm.length() == 0)
        user_nm = user_nk;

    if (is_group())
        group_id = to_string(js["group_id"]);
}
std::string notice2str(const json &j) {
    std::vector<std::string> exclude_fields = {
        "post_type", "request_type", "notice_type", "group_id",
        "user_id",   "time",         "self_id"};
    std::string formatted;
    for (auto &el : j.items()) {
        if (std::find(exclude_fields.begin(), exclude_fields.end(), el.key()) ==
            exclude_fields.end()) {
            std::string value = (el.value().type() == json::value_t::string)
                                    ? string(el.value())
                                    : el.value().dump();
            formatted += "," + el.key() + "=" + value;
        }
    }
    return formatted;
}
void Message::notice_init(const json &js) {
    message_id = "0";
    const string keys =
        js.contains("notice_type") ? "notice_type" : "request_type";
    const string notice_type = js[keys];
    auto af = [&] {
        message() = "[JM:";
        message().str() += notice_type + notice2str(js);
        message().str() += "]";
    };
    if (js.contains("group_id")) {
        set_group();
        group_id = to_string(js["group_id"]);
        user_id = js.contains("user_id") ? to_string(js["user_id"]) : "0";
        af();
    } else if (js.contains("user_id")) {
        set_private();
        user_id = to_string(js["user_id"]);
        af();
    } else {
        /// TODO
        warn_lable("[Notice_init]");
        warn_puts("暂不支持的事件");
        throw Not_Serious();
    }
}

void print_cutline() {
    string hf = "----------------";
    blue_print(hf);
    blue_print(JIEMENG_PLATFORM);
    blue_print(" ");
    blue_print(JIEMENG_VERSION);
    blue_puts(hf);
}

void Message::show() const {
    print_cutline();
    if (is_group()) {
        info_lable("[group_nm]");
        info_puts(group_nm);
        info_lable("[group_id]");
        info_puts(group_id);
    }
    info_lable("[user_nm]");
    info_puts(user_nm);
    info_lable("[user_nk]");
    info_puts(user_nk);
    info_lable("[user_id]");
    info_puts(user_id);
    msg_lable("[message]");
    msg_puts(const_str());
}
} // namespace Jiemeng
