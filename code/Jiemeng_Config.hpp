#ifndef JIEMENG_CONFIG
#define JIEMENG_CONFIG
#include <vector>
#include <nlohmann/json.hpp>
#include "Jiemeng_Algorithm.hpp"
using namespace std;
using json = nlohmann::json;
namespace Jiemeng{
    
    class Config
    {
        
        json admin_list;
        vector<string> private_list;
        vector<string> black_list;
        vector<string> group_list;
        
        bool is_private_black;
    public:
        int port;
        int sendport;
        string self_id;
        string spliter;
        unsigned int text_length;
        int time_check;
        int wait_time;
        json lua_state_list;
        
        vector<string> get_group_list(){return group_list;}
        vector<string> get_black_list(){return black_list;}
        vector<string> get_private_list(){return private_list;}
        
        void add_group_list     (const string &id) { group_list.push_back(id); }
        void add_private_list   (const string &id) { private_list.push_back(id); }
        void add_black_list     (const string &id) { black_list.push_back(id); }
        void del_group_list     (const string &id) { vec_remove_first_value(group_list, id); }
        void del_private_list   (const string &id) { vec_remove_first_value(private_list, id); }
        void del_black_list     (const string &id) { vec_remove_first_value(black_list, id); }
        bool private_white      (const string &id) const { return is_private_black ^ Vec_Find(private_list, id); }
        bool group_white        (const string &id) const { return Vec_Find(group_list, id); }
        bool is_black           (const string &id) const { return Vec_Find(black_list, id); }
        int  get_admin_level    (const string &id) const { return admin_list.count(id) ? int(admin_list[id]) : 0; }
        json custom_config;
        
        void init(const json&);
        json save();
    };
}
#endif
