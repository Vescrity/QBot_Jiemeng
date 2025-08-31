---@module 'LuaMetaLib'

mapi = mapi or {}
mapi.bot = mapi.bot or {}
bot.onebot = bot.onebot or {}
---
---@param url string
---@param api string
---@param is_Get boolean
---@param data_table table
---@param headers table
---@param return_json boolean
---@return json|table
function bot.requset_wrap(url, api, is_Get, data_table, headers, return_json)
    local req = Request.new()
    req:set_url(url)
    req:set_api(api)
    for _, str in ipairs(headers) do
        req:add_Headers(str)
    end
    local js = jsonlib.table2json(data_table)
    req:set_data(js)
    local rt
    ---@cast rt json
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
    local dt = { message_id = msg_id }
    bot.onebot_api_async('delete_msg', dt)
end
---comment
---@param message Message
---@return string
function mapi.bot.del_msg(message)
    local msgid = bot.get_reply_id(message)
    bot.onebot.del_msg(msgid)
    return ''
end

---comment
---@param gid integer
---@param uid integer
function bot.onebot.group_poke(gid, uid)
    local dt = {
        group_id = gid,
        user_id = uid
    }
    bot.onebot_api_async('group_poke', dt)
end

---@param message Message
---@return string
function mapi.bot.poke(message)
    if message:is_group() then
        bot.onebot.group_poke(message.group_id, message.user_id)
        return ''
    end
    return '未实现'
end


---comment
---@param gid integer
---@param uid integer
---@param title string
function bot.onebot.set_group_special_title(gid, uid, title)
    local dt = {
        group_id = gid,
        user_id = uid,
        special_title = title
    }
    bot.onebot_api_async('set_group_special_title', dt)
end

---comment
---@param group_id string
---@param name string
---@return string
function bot.onebot.set_group_name(group_id, name)
    local data = {
        group_id = group_id,
        group_name = name
    }
    bot.onebot_api_async('set_group_name', data)
    return ''
end
---comment
---@param msg Message
---@return string
function mapi.bot.set_group_name(msg)
    local para = msg:true_param()
    return bot.onebot.set_group_name(msg.group_id,para)
end

---comment
---@param group_id string
---@param user_id string
---@param card string
---@return string
function bot.onebot.set_group_card(group_id, user_id, card)
    local data = {
        group_id = group_id,
        user_id = user_id,
        card = card
    }
    bot.onebot_api_async('set_group_card', data)
    return ''
end

---comment
---@param group_id string
---@param user_id string
---@param duration integer
---@param async boolean
function bot.onebot.set_group_ban(group_id, user_id, duration, async)
    local data = {
        group_id = group_id,
        user_id = user_id,
        duration = duration
    }
    local f
    f = async and bot.onebot_api_async or bot.onebot_api
    f('set_group_ban', data)
end

---comment
---@param group_id string|integer
---@param character string
---@param text string
---@param async boolean
---@return json?
function bot.onebot.send_group_ai_record(group_id, character, text, async)
    local data = {
        group_id = group_id,
        character = character,
        text = text
    }
    local f
    f = async and bot.onebot_api_async or bot.onebot_api
    return f('send_group_ai_record', data)
end

---comment
---@param message_id integer
function bot.onebot.set_essence_msg(message_id)
    local data = {
        message_id = message_id,
    }
    bot.onebot_api_async('set_essence_msg', data)
end

---comment
---@param message Message
---@return string
function mapi.bot.set_title(message)
    local title = message:param()
    local gid = tonumber(message.group_id)
    ---@cast gid integer
    local uid = tonumber(message.user_id)
    ---@cast uid integer
    bot.onebot.set_group_special_title(gid, uid, title)
    return ''
end

function bot.message_output(message)
    if (message:is_group()) then
        return bot.group_output(message.group_id, message:get_string())
    else
        return bot.private_output(message.user_id, message:get_string())
    end
end

---comment
---@param message Message
---@return integer
function bot.get_reply_id(message)
    ---@type table
    local msg = jsonlib.json2table(message:get_json())
    local rt
    for _, v in ipairs(msg) do
        if (v.type == 'reply') then
            rt = v.data.id
            ---@cast rt integer
            return rt
        end
    end
    return 0
end

---comment
---@param msgid integer
---@param isjson? boolean
---@return table | json
function bot.get_msg_by_id(msgid, isjson)
    local t = { message_id = msgid }
    local qwq = bot.onebot_api('get_msg', t)
    if (isjson) then return qwq.data end
    ---@type table<string, table>
    ---@diagnostic disable-next-line: assign-type-mismatch
    local rt = jsonlib.json2table(qwq)
    return rt.data
end
---@deprecated
bot.get_msg = bot.get_msg_by_id

---comment
---@param message Message
---@return Message
function bot.get_reply_message(message)
    local msgid = bot.get_reply_id(message)
    local js = bot.get_msg_by_id(msgid, true)
    local msg = Message:new()
    msg:change(js.message)
    msg.message_id = js.message_id:val()
    msg.user_nk = tostring(js.sender.nickname:val())
    msg.user_id = tostring(js.sender.user_id:val())
    return msg
end

---@deprecated
---@param text string
---@return integer?
function find_reply_id(text)
    local id = string.match(text, "%[CQ:reply,id=(%-?%d+)%]")
    return tonumber(id)
end


---@deprecated
get_msg = bot.get_msg
