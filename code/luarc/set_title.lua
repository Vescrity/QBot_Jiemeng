function bot._set_title(gid, uid, title)
    local dt={
        action = 'set_group_special_title',
        params = {
            group_id=gid,
            user_id=uid,
            special_title=title
        }
    }
    bot._ws_send(dt)
    return ''
end

function bot.set_title(message)
    local title = get_para(message:true_str())
    local gid=tonumber(message.group_id)
    local uid=tonumber(message.user_id)
    return bot._set_title(gid, uid, title)
end
