function bot.basic_draw_deck(message, key, times)
    times = times or 1
    rt = ''
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
