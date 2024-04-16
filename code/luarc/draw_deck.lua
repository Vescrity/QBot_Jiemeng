function bot.basic_draw_deck(message, key, times)
    times = times or 1
    times = tonumber(times)
    rt = ''
    if (times==nil or times > DECK_MAX or times <= 0) then return '无效的参数值。' end
    for i = 1, times do
        local raw = bot._draw_deck(key)
        rt = rt .. bot.message_replace(raw, message.place) .. '\n'
    end
    return rt
end

function bot.draw_deck(message)
    --  @param Message message
    --  @return string 抽取结果
    local key = get_para(message:true_str())
    local times, keys = get_para(key)
    return bot.basic_draw_deck(message, keys, times)
end
