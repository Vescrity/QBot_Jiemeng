#include "Jiemeng.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_IO.hpp"
#include "txt2img_api.hpp"
// TODO: 允许用户定义 lua 函数在消息发送前处理消息内容
using namespace Jiemeng;
bool Bot::private_output(const string &user_id, const CQMessage &message) {
    try {
        auto nn = message.const_str().length();
        if (nn == 0 || user_id == "10000")
            return false;
        blue_lable("[Output]");
        if (nn < 1e4)
            blue_puts(message.const_str());
        else
            warn_puts("< Very Long, maybe binary? >");
        json send_data, rt;
        // send_data["action"] = "send_private_msg";
        send_data["user_id"] = stoull(user_id);
        send_data["message"] = message.get_json();
        rt = onebot_api("send_private_msg", send_data);
        if (rt["status"] == "ok") {
            info_lable("[Output]");
            info_puts("OK!");
            return true;
        }
        return false;
    } catch (exception &e) {
        JM_EXCEPTION("[Output]")
        return false;
    }
}
bool Bot::group_output(const string &group_id, const CQMessage &message) {
    try {
        auto nn = message.const_str().length();
        if (nn == 0)
            return false;
        blue_lable("[Output]");
        if (nn < 1e4)
            blue_puts(message.const_str());
        else
            warn_puts("< Very Long, maybe binary? >");
        json send_data, rt;
        send_data["group_id"] = stoull(group_id);
        send_data["message"] = message.get_json();
        rt = onebot_api("send_group_msg", send_data);
        if (rt["status"] == "ok") {
            info_lable("[Output]");
            info_puts("OK!");
            return true;
        }
        return false;
    } catch (exception &e) {
        JM_EXCEPTION("[Output]")
        return false;
    }
}
bool Bot::message_output(const Message_Place &place, const CQMessage &message) {
    string str = message.const_str();
    string t_str = message.true_str();
    auto l = message.true_str().length();
    if (l > config.text_length) {
        str = txt2img(t_str);
    }
    CQMessage ms(str);
    if (place.is_group())
        return group_output(place.group_id, ms);
    return private_output(place.user_id, ms);
}
bool Bot::message_output(const Message &message) {
    return message_output(message, message);
}
