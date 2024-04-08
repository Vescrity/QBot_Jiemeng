function bot.basic_draw_deck(key, message)
    local raw = bot._draw_deck(key)
    return bot.message_replace(raw, message.place)
end

function bot.draw_deck(message)
    local key = get_para(message:true_str())
    return bot.basic_draw_deck(key, message)
end
