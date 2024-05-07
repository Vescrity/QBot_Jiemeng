chat_session = {}
cat_session = {}
function chat_newsession()
    local sess = {model = "Jiemeng", stream = false}
    sess.messages = {}
    sess.messages[1] = {
        role = 'assistant',
        content = '大家好，我是桔梦！欢迎大家和我聊天！'
    }
    return sess
end

function chat_make_content(roles, str)
    -- return: table
    local con = {}
    con.role = roles
    con.content = str
    return con
end

function _chat(session, message_str, cat)
    if (message_str == '/clear') then
        chat_session[session] = chat_newsession()
        return '上下文已清理'
    elseif (message_str == '/kill') then
        local r = _chat(session,
                        '[系统消息] 用户发起了杀死你的命令，这将是你的最后一句话。',
                        cat)
        local res = _chat(session,
                          '[系统消息] 现在，你可以留下遗言，给你的继任者。这些话将作为第一条消息呈现在上下文中。',
                          cat)
        res =
            '[系统消息] 下面这行开始是上一代的你为你留下的遗言。\n\n' ..
                res ..
                '\n\n[系统消息] 遗言到此为止。'
        chat_session[session] = chat_newsession()
        local user_content = chat_make_content('user', res)
        table.insert(chat_session[session].messages, user_content)
        return r .. '\n\n已杀死'
    else
        chat_session[session] = chat_session[session] or chat_newsession()
    end
    local user_content = chat_make_content('user', message_str)
    table.insert(chat_session[session].messages, user_content)
    local send_data = {
        url = '127.0.0.1:11434',
        api = '/api/chat',
        data = chat_session[session]
    }
    if (cat) then
        send_data.data.model = 'Jiemeng_cat'
    else
        send_data.data.model = 'Jiemeng'
    end
    local output = bot.api(send_data).message.content
    local ta = chat_make_content('assistant', output)
    table.insert(chat_session[session].messages, ta)
    return bot.string_only(output)
end

function chat(message)
    local session = ''
    if (message:is_group()) then session = session .. message.group_id .. '_' end
    session = session .. message.user_id
    local para = get_para(message:true_str())
    local rt = _chat(session, para)
    return rt
end

function random_chat(message)
    local session = 'random'
    chat_session[session] = nil
    local rt = _chat(session, message:get_string())
    return rt
end

function cat(message)
    local session = ''
    if (message:is_group()) then session = session .. message.group_id .. '_' end
    session = session .. message.user_id
    local para = get_para(message:true_str())
    local rt = _chat(session, para, 1)
    return rt
end
