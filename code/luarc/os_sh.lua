function bot.os_sh(cmd)
    local command = 'COLUMNS=70 ' .. cmd .. ' 2>&1'
    local handle = io.popen(command)
    local result = handle:read("*a") 
    handle:close()
    return result 
end
