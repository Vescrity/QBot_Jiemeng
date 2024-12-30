---@diagnostic disable undefined-global

---
---@param str string
---@return string
function bot.group_broad(str)
    local broad_list = broad_list or bot.get_group_list()
    local broad_sleep = broad_sleep or 1111
    for i,v in ipairs(broad_list) do
        bot.group_output(v,str)
        bot.sleep(broad_sleep)
    end
    return ''
end
