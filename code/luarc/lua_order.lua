function lua_order_explain(script)
    local n = string.find(script, '#')
    local scriptToExecute = string.sub(script, n+1)
    if scriptToExecute then
        local func, err = load(scriptToExecute)
        if not func then
            error("Error loading script: " .. err)
        else
            return func()
        end
    else
        error("No script found after '#' in the input.")
    end
end
function lua_order(order)
    local t = lua_order_explain(order.text)
    order.text = tostring(t)
    message_output(order)
end
