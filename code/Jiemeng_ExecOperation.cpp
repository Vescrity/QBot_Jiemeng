#include "Jiemeng.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_MessageReplace.hpp"
#include "Jiemeng_Operation.hpp"
#include <sol/object.hpp>
namespace Jiemeng {
using Type = Operation::Type;
string order_parse(const Message &, const Operation &);
string Bot::exec_operation(const Message &message, const Operation &operation) {
    switch (operation.type) {
    case Type::message:
        return (operation.str);
    case Type::sleep:
        minisleep(int(operation.data));
        return "";
    case Type::lua_call:
        return lua->call(operation.str, message);
    case Type::lua_call1: {
        auto one = std::make_unique<Lua_Shell>(this, "1");
        string rt = one->call(operation.str, message);
        return rt;
    }
    case Type::lua_oper: {
        auto op = (lua->run(operation.str, message)).as<Operation>();
        return exec_operation(message, op);
    }
    case Type::lua_oper1: {
        auto one = std::make_unique<Lua_Shell>(this);
        auto op = one->run(operation.str, message).as<Operation>();
        return exec_operation(message, op);
    }
    case Type::lua_exec:
        return (lua->exec(operation.str, message));
    case Type::lua_exec1: {
        auto one = std::make_unique<Lua_Shell>(this);
        string rt = one->exec(operation.str, message);
        return rt;
    }
    case Type::state_exec: {
        auto &s = map_lua[operation.str];
        return s->exec(operation.data["exec"], message);
    }
    case Type::state_call: {
        auto &s = map_lua[operation.str];
        return s->call(operation.data["call"], message);
    }
    case Type::clear:
        throw Operation::Clear();
    case Type::stop:
        throw Operation::Stop();
    case Type::order: {
        Operation oper = operation;
        auto para = string_cut(oper.str, this->config.spliter, 2);
        auto &paras = (*para);
        string &order = paras[0];
        if (order == "s_sh") {
            auto Pa = string_cut(paras[1], this->config.spliter, 2);
            const auto &state = (*Pa)[0];
            const auto &code = (*Pa)[1];
            return this->map_lua[state]->exec(code);
        }
        if (order == "l_sh") {
            const auto &Pa = paras[1];
            return this->lua->exec(Pa);
        }
        if (order == "1_sh") {
            const auto &Pa = paras[1];
            auto l = std::make_unique<Lua_Shell>(this);
            const auto &r = l->exec(Pa);
            return r;
        }
        if (order == "sh") {
            const auto &Pa = paras[1];
            return execmd(Pa);
        }
        if (order == "answer_reload") {
            answer_reload();
            return "";
        }
        if (order == "deck_reload") {
            deck_reload();
            return "";
        }
        if (order == "type_rePost") {
            auto Para = string_cut(paras[1], config.spliter, 3);
            auto &Paras = *Para;
            message_replace(Paras[2], message);
            Operation op;
            op.str = Paras[2];
            op.type = Type::message;
            Message msg;
            if (Paras[0] != "0") {
                msg.set_group();
                msg.group_id = Paras[0];
            } else {
                msg.set_private();
                msg.user_id = Paras[1];
            }
            Operation_List opl;
            opl += op;
            process_operation(msg, opl);
            return "";
        }
        if (order == "reRecv") {
            throw Operation::reRecv(paras[1]);
        }
        throw invalid_argument("未知的 Order");
    }
    case Type::ignore: return "";
    case Type::draw_deck:
    default:
        throw runtime_error("BUG: 不应出现的 Operation");
    }
}
} // namespace Jiemeng

