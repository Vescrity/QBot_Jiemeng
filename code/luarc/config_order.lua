function bot.add_group(message)
    local para = get_para(message:true_str())
    bot.add_group_list(para)
    return ''
end
function bot.del_group(message)
    local para = get_para(message:true_str())
    bot.del_group_list(para)
    return ''
end
function bot.add_private(message)
    local para = get_para(message:true_str())
    bot.add_private_list(para)
    return ''
end
function bot.del_private(message)
    local para = get_para(message:true_str())
    bot.del_private_list(para)
    return ''
end
function bot.add_black(message)
    local para = get_para(message:true_str())
    bot.add_black_list(para)
    return ''
end
function bot.del_black(message)
    local para = get_para(message:true_str())
    bot.del_black_list(para)
    return ''
end
