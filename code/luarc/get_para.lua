function get_para(s, spliter)
    -- input: 'abc#def' '#'
    -- return: 'def' 'abc'
    spliter = spliter or bot.spliter
    local pos = s:find(spliter)
    if pos then
        return s:sub(pos + #spliter), s:sub(1, pos - 1)
    else
        return '', s
    end
end
