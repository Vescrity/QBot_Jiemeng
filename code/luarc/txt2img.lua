require('bot_string')
local reverse_split = bot.string.reverse_split
mapi = mapi or {}
---comment
---@param message Message
---@return string
function mapi.txt2img(message)
    local msg = reverse_split(message:true_str())
    return bot.txt2img(msg) or ''
end
