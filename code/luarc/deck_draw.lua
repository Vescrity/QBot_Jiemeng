function bot.basic_deck_draw(key, message)
    local raw = bot._deck_draw(key)
    return bot.message_replace(raw, message.place)
end

function bot.deck_draw(message)
    local key = get_para(message:true_str())
    return bot.basic_deck_draw(key, message)
end
