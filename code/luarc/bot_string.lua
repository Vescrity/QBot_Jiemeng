bot.string = bot.string or {}
---
---@param s string
---@param spliter? string
---@return string tail
---@return string head
function bot.string.reverse_split(s, spliter)
    spliter = spliter or bot.spliter
    local pos = s:find(spliter)
    if pos then
        return s:sub(pos + #spliter), s:sub(1, pos - 1)
    else
        return '', s
    end
end
