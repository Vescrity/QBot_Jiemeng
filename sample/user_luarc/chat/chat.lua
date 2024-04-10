chat_session = {}
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

function _chat(session, message_str)
    if (message_str == '/clear') then
        chat_session[session] = chat_newsession()
        return '上下文已清理'
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
    local output = bot.api(send_data).message.content
    local ta = chat_make_content('assistant', output)
    table.insert(chat_session[session].messages, ta)
    return output
end

function chat(message)
    local session = ''
    if (message:is_group()) then
        session = session .. message.place.group_id .. '_'
    end
    session = session .. message.place.user_id
    local para = get_para(message:true_str())
    local rt = _chat(session, para)
    return rt
end
