mapi = mapi or {}

---
---@param msg_array table
---@return string
local function get_imgurl(msg_array)
    local rt = ''
    for i, v in ipairs(msg_array) do
        if (v.type == 'image') then rt = rt .. v.data.url .. '\n' end
    end
    return rt
end
---
---@param message Message
---@return string
function mapi.get_imgurl(message)
    local msgid = bot.get_reply_id(message)
    --local msgid = find_reply_id(message:get_string())
    ---@type table
    local dt = get_msg(msgid)
    return get_imgurl(dt.message)
end
---comment
---@param message Message
---@return string
function mapi.msg_echo(message)
    -- 复读回复中指定消息
    --local id = find_reply_id(message:get_string())
    local id = bot.get_reply_id(message)
    local js = get_msg(id, true)
    local msg = Message:new()
    msg:change(js.message)
    return msg:get_string()
end
---comment
---@param message Message
---@return string
function mapi.get_info(message)
    -- 显示回复中指定的消息的详细信息
    local msgid = bot.get_reply_id(message)
    --local msgid = find_reply_id(message:get_string())
    local dt = get_msg(msgid, true)
    return dt:dump(2)
end
---comment
---@param message Message
---@return string
function mapi.del_msg(message)
    -- 尝试撤回回复的消息
    --local msgid = find_reply_id(message:get_string())
    local msgid = bot.get_reply_id(message)
    bot.onebot.del_msg(msgid)
    return ''
end

function echo_recall(message)
    -- 复读撤回的内容
    local ids = string.match(message:get_string(),
                             "%[JM:group_recall,.*message_id=(%-?%d+).*%]")
    ---@type integer
    local id = tonumber(ids)
    local js = get_msg(id, true)
    local msg = Message:new()
    msg:change(js.message)
    return msg:get_string()
end
