function bot.status()
    local rt = bot._platform .. ' ' .. bot._version .. '\n'
    rt = rt..'启动时间: '..bot.start_up_time()..'\n'
    rt = rt..'已加载词库: '..bot.deck_size()..'\n'
    return rt
end
