---comment
---@param str string
---@return Operation
function bot.txt2oper(str)
    local oper = Operation.new()
    oper.str = str
    oper:set_type('message')
    return oper
end

---comment
---@param str string
---@return Operation_List
function bot.txt2operlist(str)
    local list = Operation_List.new()
    list:push_back(bot.txt2oper(str))
    return list
end

