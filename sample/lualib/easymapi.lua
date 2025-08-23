mapi = mapi or {}
---comment
---@param message Message
---@return string
function mapi.too_Great(message)
    local str=message:get_string()
    local result = string.gsub(str, "太", "大")
    return result
end
---comment
---@param message Message
---@return string
function mapi.change_Rcode(message)
    local para = message:true_param()
    local result = bot.change_Rcode('['..para..']')
    return bot.string_only(result)
end

---comment
---@param message Message
function mapi.echo_message(message)
    bot.group_output(message.group_id, message:get_string())
end
