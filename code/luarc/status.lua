function bot.status()
    local rt = bot._platform .. ' ' .. bot._version .. '\n'
    rt = rt .. '启动时间: ' .. bot.start_up_time() .. '\n'
    rt = rt .. '已加载牌堆词条: ' .. bot.deck_size() .. '\n'
    rt = rt .. '已加载应答组: ' .. bot.answer_size() .. '\n'
    rt = rt .. '参数分隔符：' .. "'" .. bot.spliter .. "'" .. '\n'
    rt = rt .. '前端信息: \n'
    local qwq = {action = 'get_version_info', params = {}}
    s = bot.ws_send(qwq).data:dump(2)
    return rt .. s
end
