#include "Jiemeng.hpp"
#include "Jiemeng_Deck.hpp"
#include "Jiemeng_Lua.hpp"
#include "Jiemeng_Socket.hpp"
#include "Jiemeng_Http.hpp"
#include <fstream>
#include <future>
using namespace std;
using namespace Jiemeng;
void Bot::init() {
    int st = clock();
    debug_lable("[INIT]");
    debug_puts("开始执行 Config_init");
    config_init();
    auto fd = async(launch::async, [] { return new Deck; });
    auto fans = async(launch::async, [this] { answer_init(); });
    debug_lable("[INIT]");
    debug_puts("开始执行 lua_init");
    lua_init();
    debug_lable("[INIT]");
    debug_puts("开始执行 server_init");
    server_init();
    fans.get();
    deck = fd.get();
    char *buf = new char[1 << 10];
    sprintf(buf, "初始化成功！共耗时%.3lfms",
            (clock() - st) * 1000.0 / CLOCKS_PER_SEC);
    info_lable("[INIT]");
    info_puts(buf);
    delete[] buf;
}

void Bot::deck_reload() {
    Deck *p = new Deck;
    Deck *r = deck;
    deck = p;
    delete r;
}

void Bot::answer_init() {
    debug_lable("[INIT]");
    debug_puts("开始初始化应答库");
    answer.init(config.custom_config);
    debug_lable("[INIT]");
    debug_puts("应答库初始化成功！");
}
void Bot::answer_reload() {
    answer.main_answer_reload(config.custom_config);
}

void Bot::config_init() {
    ifstream fread("config.json");
    config.init(json::parse(fread));
}
void Bot::lua_init() {
    lua = new Lua_Shell(this);
    for (auto &state : config.lua_state_list) {
        auto s = map_lua[state["name"]] = new Lua_Shell(this);
        s->load(state["path"]);
    }
}
void Bot::server_init() {
    // TODO: HTTP Support
    WS_Backend* sv = new WS_Backend;
    //sv->init();
    sv->set_sendport(config.sendport);
    sv->init("127.0.0.1", config.port);
    server = sv;
}
