require('bot_string')
local reverse_split = bot.string.reverse_split
bot.mapi = bot.mapi or {}
--- 通用处理函数
---@param message Message
---@param action fun(para: string) 处理函数
---@return string
local function process_message(message, action)
    local para = reverse_split(message:true_str())
    if para == '' then return '未提供对象' end
    action(para)
    return ''
end
---
---@param message Message
---@return string
function bot.mapi.add_group(message)
    return process_message(message, bot.add_group_list)
end
---
---@param message Message
---@return string
function bot.mapi.del_group(message)
    return process_message(message, bot.del_group_list)
end
---
---@param message Message
---@return string
function bot.mapi.add_private(message)
    return process_message(message, bot.add_private_list)
end
---
---@param message Message
---@return string
function bot.mapi.del_private(message)
    return process_message(message, bot.del_private_list)
end
---
---@param message Message
---@return string
function bot.mapi.add_black(message)
    return process_message(message, bot.add_black_list)
end
---
---@param message Message
---@return string
function bot.mapi.del_black(message)
    return process_message(message, bot.del_black_list)
end
