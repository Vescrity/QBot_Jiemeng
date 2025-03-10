#include "Jiemeng_Config.hpp"
#include "Jiemeng_Exception.hpp"
#include "Jiemeng_JsonTemplate.hpp"
#include "Jiemeng_DebugIO.hpp"
bool Debug_Mode=1;
namespace Jiemeng{
    

    void Config::init(const json &js)
    {
        try
        {
            js_getval(port,                       js, "port"                                            );
            js_getval(sendport,                   js, "sendport"                                        );
            js_getval(wait_time,                  js, "wait_time",                  2000                );
            //js_getval(delay_time,                 js, "delay_time",                 2000                );
            js_getval(time_check,                 js, "time_check",                 30000               );
            //js_getval(Self_ID,                    js, "Self_ID",                    "0"s                );
            js_getval(text_length,                js, "text_length",                1u << 10            );
            js_getval(spliter,                    js, "spliter",                    "#"s                );
            //js_getval(MAX_SINGLE_MESSAGE_LENGTH,  js, "MAX_SINGLE_MESSAGE_LENGTH",  1 << 11             );
            //js_getval(MAX_MESSAGE_LENGTH,         js, "MAX_MESSAGE_LENGTH",         1 << 14             );
            //js_getval(TITLE,                      js, "TITLE",                      "Jiemeng"s + Self_ID);
            //js_getval(pswd,                       js, "pswd",                       1234                );
            js_getval(Debug_Mode,                 js, "Debug_Mode",                 false               );
            is_private_black = bool(js["private_black"]);
            admin_list = js["admin_list"];
            private_list = js["private_list"].get<vector<string>>();
            black_list = js["black_list"].get<vector<string>>();
            group_list = js["group_list"].get<vector<string>>();
            custom_config = js["Custom_Config"];
            if(js.count("lua_state_list"))
            {
                lua_state_list = js["lua_state_list"];
                if(!lua_state_list.is_array())
                    throw invalid_argument("lua_state_list 被提供了错误的数据类型。应为 array。");
            }
            
            /*if (js.count("broad_List"))
             *     broad_list = js["broad_list"].get<vector<string>>();*/
        }
        catch(const std::exception &e)
        {
            JM_EXCEPTION("[Config]");
        }
    }
    
    json Config::save()
    {
        json rt;
        rt["port"] = port;
        rt["sendport"] = sendport;
        rt["wait_time"] = wait_time;
        rt["time_check"] = time_check;
        rt["text_length"] = text_length;
        rt["spliter"] = spliter;
        rt["Debug_Mode"] = Debug_Mode;
        rt["private_black"] = is_private_black;
        rt["private_list"] = private_list;
        rt["admin_list"] = admin_list;
        rt["black_list"] = black_list;
        rt["group_list"] = group_list;
        rt["Custom_Config"] = custom_config;
        rt["lua_state_list"] = lua_state_list;
        return rt;
    }
  
}
