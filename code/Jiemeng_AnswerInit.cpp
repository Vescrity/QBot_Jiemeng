#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Answer.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_IO.hpp"
#include "Jiemeng_Operation.hpp"
#include <filesystem>
#include <fstream>
#include <memory>
#include <sol/forward.hpp>
#include <sol/sol.hpp>
#include <sstream>
namespace fs = std::filesystem;
namespace Jiemeng {

using Type = Operation::Type;
json ans_merge(const std::string &folderPath);
void Answer_List::init(const json &js) {
    clear();
    for (auto &i : js["Answers"]) {
        unique_ptr<Answer_Group> ans = make_unique<Answer_Group>(i);
        answer_group.push_back(std::move(ans));
    }
    sort(answer_group.begin(), answer_group.end(),
         [](const unique_ptr<Answer_Group> &a,
            const unique_ptr<Answer_Group> &b) { return *a < *b; });
}
unique_ptr<Answer_List> main_answer_read(const json &custom) {
    auto p = make_unique<Answer_List>();
    std::ostringstream oss;
    oss << ans_merge("./Answer");
    string strdata = oss.str();
    strdata = string_format_with_json("{~", "}", custom, strdata);
    p->init(json::parse(strdata));
    return p;
}
void All_Answer::main_answer_reload(const json &custom) {
    main_answer = main_answer_read(custom);
    return;
}
void All_Answer::init(const json &custom) {
    try {
        clear();
        main_answer = main_answer_read(custom);
    } catch (std::exception &e) { JM_EXCEPTION("[Answer_init]") }
}

void Answer_Group::init(const json &js) {
    try {
        if (js.contains("grps")) {
            if (js["grps"].is_array())
                grps = js["grps"].get<vector<string>>();
            else
                throw std::invalid_argument(
                    "argument \"grps\" should be an array");
        } else
            grps.push_back("private_true"s);
        if (js.contains("user")) {
            if (js["user"].is_array())
                user = js["user"].get<vector<string>>();
            else
                throw std::invalid_argument(
                    "argument \"user\" should be an array");
        } else
            user.push_back("1"s);
        js_getval(level, js, "level", 0);
        // 默认要求同时满足条件
        js_getval(both_flag, js, "both", true);
        if (js.contains("regex"))
            regex = Json_Regex(js["regex"]);
        else
            throw std::invalid_argument("argument \"regex\" is missing: \n");
        if (js.contains("pri"))
            priority = js["pri"];
        else
            throw std::invalid_argument("argument \"pri\" is missing: \n");

        if (js.contains("anss"))
            answer.init(js["anss"]);
        else
            throw std::invalid_argument("argument \"anss\" is not given");

    } catch (const std::exception &e) {
        throw std::invalid_argument(
            "unexpected error: " + std::string(e.what()) + "\nJSON content:\n" +
            js.dump(2));
    }
}

void Answer::init(const json &js) {
    and_flag = 0;
    leaf = 0;
    if (js.is_string()) {
        leaf = 1;
        operation.type = Operation::Type::message;
        operation.str = js;
    } else if (js.is_array()) {
        Array_init(js);
    } else if (js.is_object()) {
        if (js.contains("and")) {
            and_flag = true;
            Array_init(js["and"]);
        } else if (js.contains("or")) {
            and_flag = false;
            Array_init(js["or"]);
        } else {
            // TODO: 优化写法！
            leaf = 1;
            if (js.contains("order")) {
                operation.type = Operation::Type::order;
                if (!js["order"].is_string())
                    throw invalid_argument("order 被赋了错误的类型。");
                operation.str = js["order"];
                if (operation.str == "ignore") {
                    operation.type = Operation::Type::ignore;
                } else if (operation.str == "clear") {
                    operation.type = Operation::Type::clear;
                }
            } else if (js.contains("draw_deck")) {
                operation.type = Operation::Type::draw_deck;
                if (!js["draw_deck"].is_string())
                    throw invalid_argument("draw_deck 被赋了错误的类型。");
                operation.str = js["draw_deck"];
            } else if (js.contains("text")) {
                operation.type = Operation::Type::message;
                if (!js["text"].is_string())
                    throw invalid_argument("text 被赋了错误的类型。");
                operation.str = js["text"];
            } else if (js.contains("sleep")) {
                operation.type = Operation::Type::sleep;
                operation.data = js["sleep"];
                if (!operation.data.is_number()) {
                    try {
                        if (operation.data.is_string()) {
                            operation.data = stoll(string(operation.data));
                        } else
                            throw "";
                    } catch (...) {
                        throw invalid_argument("Sleep expect NUMBER");
                    }
                }
            } else if (js.contains("ignore")) {
                operation.type = Operation::Type::ignore;
            } else if (js.contains("clear")) {
                operation.type = Operation::Type::clear;
            }
#define ELSE_OPER(key)                                                         \
    else if (js.contains(#key)) {                                              \
        operation.type = Operation::Type::key;                                 \
        operation.str = js[#key];                                              \
    }
            ELSE_OPER(lua_call)
            ELSE_OPER(lua_call1)
            ELSE_OPER(lua_exec)
            ELSE_OPER(lua_exec1)
            ELSE_OPER(lua_oper)
            ELSE_OPER(lua_oper1)
            ELSE_OPER(state_call)
            ELSE_OPER(state_exec)
#undef ELSE_OPER
            else {
                warn_lable("[ELSE_OPER]");
                warn_puts("undefined Operation?");
                warn_puts(js.dump(2));
            }
            if (js.contains("data")) {
                operation.is_json = 1;
                operation.data = js["data"];
            }
        }
    }
    // ignore 替换
    if (operation.type == Type::message) {
        if (operation.str == "[ignore]")
            operation.type = Type::ignore;
    }
}

void Answer::Array_init(const json &js) {
    // int cnt = js.size();
    if (is_or()) {
        lvs.push_back(0);
        for (auto &i : js) {
            if (!i.is_object())
                lvs.push_back(lvs.back() + 1);
            else if (!i.contains("weight"))
                lvs.push_back(lvs.back() + 1);
            else
                lvs.push_back(int(i["weight"]) + lvs.back());
            auto ans = make_unique<Answer>();
            ans->init(i);
            sub_ans.push_back(std::move(ans));
        }
    } else {
        for (auto &i : js) {
            auto ans = make_unique<Answer>();
            ans->init(i);
            sub_ans.push_back(std::move(ans));
        }
    }
}

json ans_merge(const std::string &folderPath) {
    sol::state lua_s;
    lua_s.open_libraries();
    sol::function table2json =
        lua_s.script(R"( return require("cjson").encode)");
    json mergedJson;
    std::vector<json> answerArrays;
    for (const auto &entry : fs::directory_iterator(folderPath)) {
        json fileJson;
        if (entry.path().extension() == ".lua") {
            auto t = lua_s.do_file(entry.path());
            assert(t.get_type() == sol::type::table);
            string s = table2json(t);
            fileJson = json::parse(s);
        } else if (entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                try {
                    file >> fileJson;
                    file.close();
                } catch (exception &e) {
                    error_lable("[Answer_Merge]");
                    error_puts("读取文件 " + string(entry.path()) +
                               " 时发生错误：" + e.what());
                    file.close();
                }
            } else {
                throw runtime_error("Failed to open file: "s +
                                    string(entry.path()));
            }
        } else
            continue;
        if (fileJson.contains("Answers") && fileJson["Answers"].is_array())
            answerArrays.push_back(fileJson["Answers"]);
        else {
            warn_lable("[Answer_Merge]");
            warn_puts("文件 "s + string(entry.path()) +
                      " 没有发现 Answers "
                      "字段或格式不正确，请检查。本次加载将忽略。");
        }
    }

    mergedJson["Answers"] = json::array();
    for (const auto &answers : answerArrays) {
        mergedJson["Answers"].insert(mergedJson["Answers"].end(),
                                     answers.begin(), answers.end());
    }
    return mergedJson;
}
} // namespace Jiemeng
