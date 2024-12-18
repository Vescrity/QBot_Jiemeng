---@meta

---TODO: GPT 生成，类型未修改，内容不全

---@class json
json = {}

--- 创建一个新的 json 对象
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

--- 将一个 Lua 对象解析为 json
--- @param obj any 任何 Lua 对象
function json:parse(obj) end

--- 将 json 对象转换为 Lua 表
--- @return table
function json:val() end

----------------------------------

---@class Request
Request = {}

--- 创建一个新的 Request 对象
--- @return Request
function Request.new() end

--- 设置请求的 URL
--- @param url string 要设置的 URL
function Request:set_url(url) end

--- 设置请求的 API
--- @param api string 要设置的 API
function Request:set_api(api) end

--- 设置请求的数据
--- @param data any 要设置的数据
function Request:set_data(data) end

--- 设置请求的消息
--- @param msgs any 要设置的消息
function Request:set_msgs(msgs) end

--- 执行 POST 请求
--- @return any 返回的结果
function Request:Post() end

--- 执行 JavaScript 的 POST 请求
--- @return any 返回的结果
function Request:js_post() end

--- 执行 JavaScript 的 GET 请求
--- @return any 返回的结果
function Request:js_get() end

--- 执行 GET 请求
--- @return any 返回的结果
function Request:Get() end

---@class bot
bot = {}

--- sleep (ms)
--- @param ms number 
function bot.sleep(ms) end

--- 生成一个随机数
--- @return number 返回生成的随机数
function bot.rand(l, r) end

--- 将字符串转换为 CQ 码 (\&->\&amp; 等)
--- @param str string 要转换的字符串
--- @return string 转换后的字符串
function bot.string_only(str) end

--- 向指定群组发送消息
--- @param group_id string 群组 ID
--- @param message string 要发送的消息
--- @return boolean 返回操作是否成功
function bot.group_output(group_id, message) end

--- 向指定用户发送消息
--- @param user_id string 用户 ID
--- @param message string 要发送的消息
--- @return boolean 返回操作是否成功
function bot.private_output(user_id, message) end

--- 从牌组中抽取一张牌
--- @param key string 牌组的名称或键
--- @return any 返回抽取的牌
function bot._draw_deck(key) end

--- 列出当前牌组中的所有牌
--- @return table 返回当前牌组的列表
function bot.deck_list() end

--- 替换消息中的某些内容
--- @param str string 原始字符串
--- @param message Message 要处理的消息
--- @return string 返回替换后的字符串
function bot.message_replace(str, message) end

--- 处理接收到的消息
--- @param message Message 要处理的消息
--- @return any 返回处理的结果
function bot.process_message(message) end

--- 按照指定操作列表处理消息
--- @param message Message 要处理的消息
--- @param list Operation_List 操作列表
--- @return any 返回处理的结果
function bot.process_operation(message, list) end

--- 重新加载回答
--- @return any 返回重新加载的结果
function bot.answer_reload() end

--- 重新加载牌组
--- @return any 返回重新加载的结果
function bot.deck_reload() end

--- 重新加载 Lua 脚本
--- @return string 返回空字符串
function bot.lua_reload() end
