function get_msg(msgid, isjs)
    local t = {action = 'get_msg', params = {message_id = msgid}}
    local qwq = bot.ws_send(t)
    if (isjs) then return qwq.data end
    local rt = jsonlib.json2table(qwq)
    return rt.data
end

function find_reply_id(text)
    local id = string.match(text, "%[CQ:reply,id=(%-?%d+)%]")
    return tonumber(id)
end

function _get_imgurl(msg_array)
    local rt = ''
    for i, v in ipairs(msg_array) do
        if (v.type == 'image') then rt = rt .. v.data.url .. '\n' end
    end
    return rt
end

function get_imgurl(message)
    local msgid = find_reply_id(message:get_string())
    local dt = get_msg(msgid)
    return _get_imgurl(dt.message)
end

function msg_echo(message)
    -- 复读回复中指定消息
    local id = find_reply_id(message:get_string())
    local js = get_msg(id, 1)
    local msg = Message:new()
    msg:change(js.message)
    return msg:get_string()
end

function get_info(message)
    -- 显示回复中指定的消息的详细信息
    local msgid = find_reply_id(message:get_string())
    local dt = get_msg(msgid, 1)
    return dt:dump(2)
end

function del_msg(message)
    -- 尝试撤回回复的消息
    local msgid = find_reply_id(message:get_string())
    return bot.del_msg(msgid)
end

function echo_recall(message)
    -- 复读撤回的内容
    local ids = string.match(message:get_string(),
                             "%[JM:group_recall,message_id=(%-?%d+)%]")
    local id = tonumber(ids)
    local js = get_msg(id, 1)
    local msg = Message:new()
    msg:change(js.message)
    return msg:get_string()
end
