---
---@param msg_id integer
---@return 
function bot.del_msg(msg_id)
    local dt = {action = 'delete_msg', params = {message_id = msg_id}}
    bot._ws_send(dt)
    return ''
end
function bot.group_poke(gid, uid)
    local dt={
        action = 'group_poke',
        params = {
            group_id=gid,
            user_id=uid
        }
    }
    bot._ws_send(dt)
end
function bot.set_group_special_title(gid, uid, title)
    local dt={
        action = 'set_group_special_title',
        params = {
            group_id=gid,
            user_id=uid,
            special_title=title
        }
    }
    bot._ws_send(dt)
end

function bot.set_title(message)
    local title = get_para(message:true_str())
    local gid=tonumber(message.group_id)
    local uid=tonumber(message.user_id)
    bot.set_group_special_title(gid, uid, title)
    return ''
end
