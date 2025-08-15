require('bot_string')
local reverse_split = bot.string.reverse_split
---comment
---@param script string
---@return string
function bot.lua_shell_explain(script)
    local scriptToExecute = reverse_split(script)
    if scriptToExecute then
        local func, err = load(scriptToExecute)
        if not func then
            error("Error loading script: " .. err)
        else
            return func()
        end
    else
        error("No script found.")
    end
end
---comment
---@param order Message
---@return string
function bot.lua_shell(order)
    return bot.lua_shell_explain(order:true_str())
end
