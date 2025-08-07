#include "Jiemeng_Operation.hpp"
#include "Jiemeng.hpp"
#include "Jiemeng_Algorithm.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Message.hpp"
#include "Jiemeng_MessageReplace.hpp"

namespace Jiemeng {
using Type = Operation::Type;

Operation_List extract(Operation oper, const Message &message, Bot *bot) {
    Operation_List rt;
    Operation clears;
    clears.type = Type::clear;
    if (oper.type == Type::message) {
        message_replace(oper.str, message);
        auto uwu = string_cut(oper.str, "[-cut-]");
        Operation op;
        op.type = Type::message;
        int qwq = 0;
        for (const auto &i : *uwu) {
            op.str = i;
            rt += op;
            if (qwq++)
                rt += clears;
        }
        return rt;
    }
    if (oper.type == Type::draw_deck) {
        auto para = string_cut(oper.str, bot->config.spliter);
        auto p1 = (*para)[0];
        int t = 1;
        if (para->size() > 1) {
            try {
                t = stoll((*para)[1]);
            } catch (...) {}
        }
        oper.str = bot->deck->draw(p1, t);
        oper.type = Type::message;
        rt += extract(oper, message, bot);
        return rt;
    }
    if (oper.type == Type::ignore) {
        return rt;
    }
    if (oper.type == Type::order) {
        auto para = string_cut(oper.str, bot->config.spliter, 2);
        auto &paras = (*para);
        const string &order = paras[0];
        if (order == "order") {
            auto Pa = string_cut(message.true_str(), bot->config.spliter, 2);
            oper.str = (*Pa)[1];
            rt += extract(oper, message, bot);
            return rt;
        }
        if (order == "sleep") {
            auto Pa = string_cut(message.true_str(), bot->config.spliter, 2);
            oper.data = stoll((*Pa)[1]);
            oper.type = Type::sleep;
            rt += oper;
            return rt;
        }

        if (order == "draw_deck") {
            oper.type = Type::draw_deck;
            oper.str = paras[1];
            rt += extract(oper, message, bot);
            return rt;
        }
        rt += oper;
        return rt;
    }
    rt += oper;
    return rt;
}

void Operation_List::upgrade(const Message &message, Bot *bot) {
    debug_lable("[Upgrade]");
    dout << "开始对 Operation_List 执行更新操作\n";
    Operation_List rt;
    for (auto &i : list)
        rt += extract(i, message, bot);
    /*Operation c;
     *   c.type = Type::clear;
     *   rt += c;
     *   list.clear();*/
    list = rt.list;
}

void Operation::set_type(const string &t) {
    if (t == "message") {
        type = Type::message;
        return;
    }
    if (t == "order") {
        type = Type::order;
        return;
    }
    if (t == "lua_call") {
        type = Type::lua_call;
        return;
    }
    if (t == "lua_once") {
        type = Type::lua_once;
        return;
    }
    if (t == "call_state") {
        type = Type::call_state;
        return;
    }
    if (t == "ignore") {
        type = Type::ignore;
        return;
    }
    if (t == "draw_deck") {
        type = Type::draw_deck;
        return;
    }
    if (t == "clear") {
        type = Type::clear;
        return;
    }
    throw invalid_argument("未定义的 Operation_Type");
}

Operation::Operation():type(Type::message), str("clear"){}
void Operation_List::clear_ignore() {
    while (!list.empty()) {
        if (list.back().type != Type::ignore)
            break;
        list.pop_back();
    }
}
} // namespace Jiemeng
