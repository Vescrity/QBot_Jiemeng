function _txt2img(gid, uid, title)
    return ''
end

function txt2img(message)
    local msg = get_para(message:true_str())
    return bot.txt2img(msg)
end
