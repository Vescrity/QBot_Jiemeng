---@module 'LuaMetaLib'

require('bot_string')
local reverse_split = bot.string.reverse_split

function Message:true_param()
    local t = reverse_split(self:true_str())
    return t
end
function Message:param()
    local t = reverse_split(self:get_string())
    return t
end
