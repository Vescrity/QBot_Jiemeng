
---@module 'LuaMetaLib'

require('bot_string')
local reverse_split = bot.string.reverse_split
mapi = mapi or {}
mapi.bot = mapi.bot or {}
bot.onebot = bot.onebot or {}
---
---@param url string
---@param api string
---@param is_Get boolean
---@param data_table table
---@param return_json boolean
---@return json|table
function bot.requset_wrap(url, api, is_Get, data_table, headers, return_json)
    local req = Request.new()
    req:set_url(url)
    req:set_api(api)
    for i, str in ipairs(headers) do
        req:add_Headers(str)
    end
    local js = jsonlib.table2json(data_table)
    req:set_data(js)
    local rt = json.new()
    if (is_Get) then
        rt = req:js_get()
    else
        rt = req:js_post()
    end
    if (return_json) then
        ---@type json
        return rt
    end
    local rrt = jsonlib.json2table(rt)
    if type(rrt) == "number" then
        error("Excp: Why the server gave us a number?")
    end
    ---@type table
    return rrt
end

---
---@param Data table
---@param return_json? boolean (false)
---@return json|table
function bot.request_api(Data, return_json)
    local get = Data.get or false
    local rtj = return_json or false
    local headers = Data.headers or {}
    return bot.requset_wrap(Data.url, Data.api, get, Data.data, headers, rtj)
end

---
---@param msg_id integer
---@return nil
function bot.onebot.del_msg(msg_id)
    local dt = {message_id = msg_id}
    bot.onebot_api_async('delete_msg', dt)
end
---comment
---@param gid integer
---@param uid integer
function bot.onebot.group_poke(gid, uid)
    local dt={
        group_id=gid,
        user_id=uid
    }
    bot.onebot_api_async('group_poke',dt)
end
---comment
---@param gid integer
---@param uid integer
---@param title string
function bot.onebot.set_group_special_title(gid, uid, title)
    local dt={
        group_id=gid,
        user_id=uid,
        special_title=title
    }
    bot.onebot_api_async('set_group_special_title', dt)
end
---comment
---@param message Message
---@return string
function mapi.bot.set_title(message)
    local title = reverse_split(message:true_str()) or ''
    ---@type integer
    local gid=tonumber(message.group_id)
    ---@type integer
    local uid=tonumber(message.user_id)
    bot.onebot.set_group_special_title(gid, uid, title)
    return ''
end

function bot.message_output(message)
    if(message:is_group()) then
        return bot.group_output(message.group_id,message:get_string())
    else
        return bot.private_output(message.user_id,message:get_string())
    end
end
---comment
---@param message Message
---@return integer
function bot.get_reply_id(message)
    ---@type table
    local msg = jsonlib.json2table(message:get_json())
    local rt = 0
    for i,v in ipairs(msg) do
        if (v.type == 'reply') then
            rt = v.data.id
            return rt
        end
    end
    return 0
end

---@deprecated
---@param text string
---@return integer?
function find_reply_id(text)
    local id = string.match(text, "%[CQ:reply,id=(%-?%d+)%]")
    return tonumber(id)
end
---comment
---@param msgid integer
---@param isjson? boolean
---@return table | json
function bot.get_msg(msgid, isjson)
    local t = {message_id = msgid}
    local qwq = bot.onebot_api('get_msg' ,t)
    if (isjson) then return qwq.data end
    ---@type table<string, table>
    ---@diagnostic disable-next-line: assign-type-mismatch
    local rt = jsonlib.json2table(qwq)
    return rt.data
end
---@deprecated
get_msg = bot.get_msg
