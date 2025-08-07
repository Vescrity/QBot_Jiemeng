#include "Jiemeng.hpp"
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_DebugIO.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_IO.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_Operation.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Time.hpp"
#include <chrono>
#include <filesystem>
#include <future>
#include <thread>
namespace fs = std::filesystem;
using namespace std;
namespace Jiemeng {
void Bot::run() {
    std::thread([this] {
        string ti, tp;
        for (;;) {
            Time_Class tm;
            ti = tm.time_mark();
            if (ti != tp) {
                tp = ti;
                Message msg(ti);
                msg.user_id = "10000";
                msg.user_nm = "时钟消息";
                msg.set_private();
                std::thread([this, msg] {
                    this->process_message(msg);
                }).detach();
            }
            minisleep(config.time_check);
        }
    }).detach();
    
    auto pmsg = [this](const json &js) {
        try {
            {
                std::lock_guard<std::mutex> locker(lua->mtx);
                auto res = lua->get_func("pre_process");
                if (res.valid()) {
                    sol::type result_type = res.get_type();
                    if (result_type == sol::type::function) {
                        sol::function fu = res;
                        auto r = fu(js);
                        sol::type rty = r.get_type();
                        if (rty == sol::type::boolean) {
                            if (r)
                                return;
                        }
                    } else if (result_type == sol::type::nil) {
                    } else {
                        warn_lable("[Pre_Process]");
                        warn_puts("pre_process 被置为了非函数类型");
                    }
                }
            }

            Message msg = generate_message(js);
            msg.show();
            process_message(msg);
        } catch (const Not_Serious &) { return; }
    };
    server->run(pmsg);
}

void Bot::save_config() {
    FILE *fp;
    fp = fopen("config.json", "w");
    fprintf(fp, "%s", config.save().dump(2).c_str());
    fclose(fp);
}

bool dir_exists(const string &name) {
    if (fs::exists(name) && fs::is_directory(name))
        return 1;
    return 0;
}
bool file_exists(const string &name) {
    if (fs::exists(name))
        return 1;
    return 0;
}

void work_dir_check() {
    if (!dir_exists("tmp")) {
        if (fs::create_directory("tmp"))
            ;
        else
            throw runtime_error("Failed to creat dir tmp.");
    }
    if (!dir_exists("txt2img")) {
        warn_lable("[txt2img]");
        warn_puts("没有发现 txt2img 文件夹，请确保运行时不会触发相关功能。");
    }
    // TODO
    return;
}

Bot::Bot() {
    init();
}
Bot::~Bot() {
    clear();
}
void Bot::clear() {
    delete server;
    debug_lable("[Clear]");
    debug_puts("清理完毕。");
}

json Bot::onebot_api(const string &api, json &a) {
    return server->onebot_api(api, a);
}
void Bot::process_operation(const Message &message, Operation_List &list,
                            string &buf) {
    list.upgrade(message, this);
    for (auto &i : list.list) {
        try {
            buf = buf + exec_operation(message, i);
        } catch (const Operation::Clear &) {
            CQMessage ms(buf);
            message_output(message, ms);
            buf = "";
        } catch (Operation::reRecv &e) {
            Message msg = message;
            msg.change(string(e.what()));
            Operation_List rl;
            rl = answer.get_list(msg);
            process_operation(msg, rl, buf);
        } catch (exception &e) { JM_EXCEPTION("[Exec_Operation]"); }
    }
}
void Bot::process_operation(const Message &message, Operation_List &list) {
    string uwu = "";
    Operation clears;
    clears.type = Operation::Type::clear;
    list += clears;
    process_operation(message, list, uwu);
}

void Bot::process_message(Message message) {
    if (!is_white(message))
        return;
    try {
        Operation_List list = answer.get_list(message);
        process_operation(message, list);
    } catch (const Not_Serious &) {}
}
bool Bot::is_white(const Message_Place &place) {
    if (config.is_black(place.user_id))
        return false;
    if (place.is_group()) {
        if (!config.group_white(place.group_id))
            return false;
    } else {
        if (!config.private_white(place.user_id))
            return false;
    }
    return true;
}
Message Bot::generate_message(const json &js) {
    const string &post_type = js["post_type"];
    string able_type[] = {"message", "message_sent", "notice", "request"};
    if (!array_search(post_type, able_type)) {
        debug_lable("[Event]");
        debug_puts("Not a message.");
    } else {
        Message message;
        message.init(js);
        message.get_level(&config);
        if (message.is_group())
            message.group_nm = get_group_name(message.group_id);
        if (message.user_nm.length() == 0) {
            if (message.is_group())
                message.user_nm =
                    get_user_name(message.group_id, message.user_id);
            else
                message.user_nm = get_user_name(message.user_id);
        }
        return message;
    }
    throw Not_Serious();
}

string Bot::get_group_name(const string &group_id) {
    json js;
    js["group_id"] = stoull(group_id);
    auto result = async(launch::async, [this, &js] {
        return server->onebot_api("get_group_info", js);
    });
    if (result.wait_for(chrono::milliseconds(config.wait_time)) ==
        future_status::timeout) {
        warn_lable("[Get_Group_Name]");
        warn_puts("获取群名超时，放弃等待。");
        return "";
    }
    js = result.get();
    if (js["data"].is_null())
        return "";
    else if (!js["data"].contains("group_name"))
        return "";
    js = js["data"]["group_name"];
    if (js.is_null())
        return "";
    return js;
}

string Bot::get_user_name(const string &group_id, const string &user_id) {
    json js;
    js["user_id"] = stoull(user_id);
    js["group_id"] = stoull(group_id);
    js["action"] = "get_group_member_info";
    auto result = async(launch::async, [this, &js] {
        return server->onebot_api("get_group_member_info", js);
    });
    if (result.wait_for(chrono::milliseconds(config.wait_time)) ==
        future_status::timeout) {
        warn_lable("[Get_User_Name]");
        warn_puts("获取名片超时，放弃等待。");
        return "";
    }
    js = result.get();
    if (js["data"].is_null())
        return "";
    js = js["data"];
    string user_nm;
    if (js.contains("card"))
        if (js["card"].is_string())
            return js["card"];
    if (user_nm.length() == 0)
        return js["nickname"];
    else
        return user_nm;
}
string Bot::get_user_name(const string &user_id) {
    json js;
    js["user_id"] = stoull(user_id);
    // js["action"] = "get_stranger_info";
    auto result = async(launch::async, [this, &js] {
        return server->onebot_api("get_stranger_info", js);
    });
    if (result.wait_for(chrono::milliseconds(config.wait_time)) ==
        future_status::timeout) {
        warn_lable("[Get_User_Name]");
        warn_puts("获取名片超时，放弃等待。");
        return "";
    }
    js = result.get();
    if (js["data"].is_null())
        return "";
    return js["data"]["nickname"];
}
} // namespace Jiemeng
