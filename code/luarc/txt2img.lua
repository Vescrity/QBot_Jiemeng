require('bot_string')
local reverse_split = bot.string.reverse_split
mapi = mapi or {}
---comment
---@param message Message
---@return string
function mapi.txt2img(message)
    return bot.txt2img(message:true_param()) or ''
end
