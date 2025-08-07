#ifndef JIEMENG_HPP
#define JIEMENG_HPP

#include "Jiemeng_Answer.hpp"
#include "Jiemeng_Config.hpp"
#include <map>
#include <memory>
using namespace std;
namespace Jiemeng {
class Operation;
class Server;
class Lua_Shell;
class Deck;
class Bot {
    All_Answer answer;
    Config config;
    Server *server;
    unique_ptr<Deck> deck;
    unique_ptr<Lua_Shell> lua;
    map<string, unique_ptr<Lua_Shell> > map_lua;
    Message generate_message(const json &);
    void process_message(Message);
    bool message_output(const Message &);
    bool message_output(const Message_Place &, const CQMessage &);
    bool group_output(const string &group_id, const CQMessage &message);
    bool private_output(const string &user_id, const CQMessage &message);
    bool is_white(const Message_Place &);
    string get_group_name(const string &group_id);
    string get_user_name(const string &group_id, const string &user_id);
    string get_user_name(const string &user_id);
    string exec_operation(const Message &, const Operation &);

    void answer_init();
    void config_init();
    void server_init();
    /// @brief 仅限初始化使用！
    void lua_init();
    void answer_reload();
    void deck_reload();
    void save_config();
    void process_operation(const Message &, Operation_List &);
    void process_operation(const Message &, Operation_List &, string &);
    friend class Lua_Shell;
    friend Operation_List extract(Operation oper, const Message &message,
                                  Bot *bot);

  public:
    Bot();
    ~Bot();
    void init();
    void clear();
    void run();
    json onebot_api(const string &api, json &a);
};

void work_dir_check();
} // namespace Jiemeng

#endif
