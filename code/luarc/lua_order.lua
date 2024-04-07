function bot.lua_shell_explain(script)
    local scriptToExecute = get_para(script)
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
function bot.lua_shell(order)
    return bot.lua_shell_explain(order:true_str())
end
