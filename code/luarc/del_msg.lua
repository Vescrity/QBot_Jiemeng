---
---@param msg_id integer
---@return 
function bot.del_msg(msg_id)
    local dt = {action = 'delete_msg', params = {message_id = msg_id}}
    bot._ws_send(dt)
    return ''
end
