---@meta

---@class json
---@field [integer] json
---@field [string] json
json = {}

--- 创建一个新的 json 对象
--- @return json
function json.new() end

--- 将 json 对象转换为字符串
--- @param n? integer 可选参数，指定缩进级别
--- @return string
function json:dump(n) end

--- 将一个键值对添加到 json 对象
--- @param key string 键
--- @param value json 要设置的值
function json:__newindex(key, value) end

--- 从 json 对象中获取值
--- @param key string 键
--- @return json
function json:__index(key) end

--- 将一个字符串/表解析为 json
--- @param obj string|table
--- @return nil
function json:parse(obj) end

--- 将 json 对象转换为 Lua 对象
--- @return table | number | string | boolean
function json:val() end

----------------------------------
--- libcurl 包装的 Request 库
---@class Request
Request = {}

--- 创建一个新的 Request 对象
--- @return Request
function Request.new() end

--- 添加请求头
--- @param header string
--- @return nil
function Request.add_Headers(header) end

--- 设置 URL
--- @param url string URL
--- @return nil
function Request:set_url(url) end

--- 设置 API
--- @param api string API, e.g.: '/text'
--- @return nil
function Request:set_api(api) end

--- 设置传送的数据(json)
--- @param data json 
function Request:set_data(data) end

--- 设置传送的数据(string)
--- @param msgs string
function Request:set_msgs(msgs) end

--- 执行 POST 请求
--- @return string data 响应
function Request:Post() end

--- 执行 GET 请求
--- @return string data 响应
function Request:Get() end

--- POST 请求，并以 json 格式返回响应
--- @return json data 响应数据
function Request:js_post() end

--- GET 请求，以 json 格式返回响应
--- @return json data 响应数据
function Request:js_get() end

---@class Message_Place 
---@field group_id string
---@field group_nm string
---@field user_id string
---@field user_nm string
---@field level integer 权限等级
Message_Place = {}
---
---@return Message_Place 
function Message_Place.new() end
---设置为群组消息
function Message_Place:set_group() end
---设置为私信消息
function Message_Place:set_private() end
---检查是否为群组消息
---@return boolean
function Message_Place:is_group() end
---检查是否为私信消息
---@return boolean
function Message_Place:is_private() end
---获取权限等级
---@return integer
function Message_Place:get_level() end

---消息类 
---@class Message:Message_Place 
Message = {}
---@return Message 
function Message.new() end
---获取消息 CQ 码
---@return string
function Message:get_string() end
---获取消息段 json
---@return json
function Message:get_json() end
---获取消息纯文本 
---@return string
function Message:true_str() end
---获取消息位置属性 
---@return Message_Place
function Message:get_place() end
---输出消息至控制台
---@return nil
function Message:show() end
---根据传入更改消息内容
---@param msg string|json
---@return nil
function Message:change(msg) end

---@class Operation
---@field str string
Operation = {}

---@return Operation 
function Operation.new() end

---设置 Operation 的类型
---@param type_name string
function Operation:set_type(type_name) end


---@class Operation_List
Operation_List = {}

---@return Operation_List
function Operation_List.new() end
---@param oper Operation
---@return nil
function Operation_List:push_back(oper) end

bot = {}
bot._version = ''
bot._platform = ''
bot._compile_time = ''
bot.spliter = ''
bot.custom_config = {}
---@deprecated 因为不能同步更新故不建议使用
bot.group_list = {}

--- sleep (ms)
--- @param ms number 
function bot.sleep(ms) end

--- 返回[l,r]区间内随机整数
--- @param l integer
--- @param r integer
--- @return integer rand_number
function bot.rand(l, r) end
--- 对 rstr 中的 Rcode 变换
--- @param rstr string
--- @return string
function bot.change_Rcode(rstr) end

--- 将字符串转换为 CQ 码 (\&->\&amp; 等)，使之发送后的表面值等于传入值
--- @param str string 要转换的字符串
--- @return string str 处理后的字符串
function bot.string_only(str) end

--- 向指定群组发送消息
--- @param group_id string 群组 ID
--- @param message string 要发送的消息
--- @return boolean result 返回操作是否成功
function bot.group_output(group_id, message) end

--- 向指定用户发送消息
--- @param user_id string 用户 ID
--- @param message string 要发送的消息
--- @return boolean result 操作是否成功
function bot.private_output(user_id, message) end

--- 从牌组中抽取一张牌
--- @param key string 牌组的名称或键
--- @return string result 抽取结果
function bot.draw_deck(key) end

--- 列出当前牌组中的所有牌
--- @return string 返回当前牌组的列表
function bot.deck_list() end

--- 替换如 `[user_nm]` 等内容
--- @param str string 原始字符串
--- @param message Message 参照消息
--- @return string result
function bot.message_replace(str, message) end

--- 按等效接收到该消息的方式处理消息 
--- @param message Message 要处理的消息
--- @return nil 
function bot.process_message(message) end

--- 按照指定操作列表处理消息
--- @param message Message 要处理的消息
--- @param list Operation_List 操作列表
--- @return nil
function bot.process_operation(message, list) end

--- 重新加载应答组
function bot.answer_reload() end

--- 重新加载牌组
function bot.deck_reload() end

--- 重新加载 Lua 脚本 (Async)
function bot.lua_reload() end
--- 与 Onebot 实例进行 websocket 通信
---@param data table
---@return json msg 响应
function bot.ws_send(data) end
--- 与 Onebot 实例进行 websocket 通信(Async)
---@param data table
---@return nil 
function bot.ws_send_async(data) end

---@return string
function bot.start_up_time() end

---文本转图片
---@param str string [CQ:image,...]
function bot.txt2img(str) end

---@return integer
function bot.get_deck_size() end
---@return integer
function bot.get_answer_size() end

---@return string[]
function bot.get_group_list() end
---@param id string id
function bot.add_group_list(id) end
---@param id string id
function bot.add_private_list(id) end
---@param id string id
function bot.add_black_list(id) end
---@param id string id
function bot.del_group_list(id) end
---@param id string id
function bot.del_private_list(id) end
---@param id string id
function bot.del_black_list(id) end

function bot.save_config() end
---在指定的 lua state 中运行 lua 代码
---@param state_name string
---@param code string
---@return string
function bot.state_run(state_name, code) end
---在指定的 lua state 中运行 lua 代码(async)
---@param state_name string
---@param code string
function bot.state_run_async(state_name, code) end

jsonlib = {}
---@param t table
---@return json
function jsonlib.table2json(t) end
---@param j json
---@return table|number|string
function jsonlib.json2table(j) end
