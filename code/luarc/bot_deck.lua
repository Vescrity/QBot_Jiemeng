
require('bot_string')
local reverse_split = bot.string.reverse_split
DECK_MAX = DECK_MAX or 20
---
---@param message Message
---@param key string
---@param times string | integer | nil
---@return string
function bot.basic_draw_deck(message, key, times)
    times = times or 1
    times = tonumber(times)
    local rt = ''
    if (times==nil or times > DECK_MAX or times <= 0) then return '无效的参数值。' end
    for i = 1, times do
        local raw = bot._draw_deck(key)
        rt = rt .. bot.message_replace(raw, message) .. '\n'
    end
    return rt:gsub("%s+$","")
end
---
---@param message Message
---@return string
function bot.mapi.draw_deck(message)
    local key = reverse_split(message:true_str())
    local times, keys = reverse_split(key)
    return bot.basic_draw_deck(message, keys, times)
end