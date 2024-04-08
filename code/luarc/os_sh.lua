function bot.os_sh(cmd)
    local command = cmd .. ' 2>&1'
    local handle = io.popen(command)
    local result = handle:read("*a") 
    handle:close()
    return result 
end
