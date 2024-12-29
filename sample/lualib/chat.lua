local chat = chat or {}
chat.chat_session = {}
chat.cat_session = {}
chat.mapi = chat.mapi or {}
require("bot_string")
require("bot_api")

local function chat_newsession()
    local sess = {model = "Jiemeng", stream = false}
    sess.messages = {}
    sess.messages[1] = {
        role = 'assistant',
        content = '大家好，我是桔梦！欢迎大家和我聊天！'
    }
    return sess
end
chat.chat_newsession = chat_newsession
---comment
---@param roles string
---@param str string
---@return table
local function chat_make_content(roles, str)
    -- return: table
    local con = {}
    con.role = roles
    con.content = str
    return con
end
chat.chat_make_content = chat_make_content
---comment
---@param session string
---@param message_str string
---@param model string
---@return string
function chat.chat(session, message_str, model)
    if (message_str == '/clear') then
        chat.chat_session[session] = chat_newsession()
        return '上下文已清理'
    elseif (message_str == '/kill') then
        local r = chat(session,
                        '[系统消息] 用户发起了杀死你的命令，这将是你的最后一句话。',
                        model)
        local res = chat(session,
                          '[系统消息] 现在，你可以留下遗言，给你的继任者。这些话将作为第一条消息呈现在上下文中。',
                          model)
        res =
            '[系统消息] 下面这行开始是上一代的你为你留下的遗言。\n\n' ..
                res ..
                '\n\n[系统消息] 遗言到此为止。'
        chat.chat_session[session] = chat_newsession()
        local user_content = chat_make_content('user', res)
        local res_content = chat_make_content('assistant', '我知道了。这是一条系统消息。')
        table.insert(chat.chat_session[session].messages, user_content)
        table.insert(chat.chat_session[session].messages, res_content)
        return r .. '\n\n已杀死'
    else
        chat.chat_session[session] = chat.chat_session[session] or chat_newsession()
    end
    local user_content = chat_make_content('user', message_str)
    table.insert(chat.chat_session[session].messages, user_content)
    local send_data = {
        url = '127.0.0.1:11434',
        api = '/api/chat',
        data = chat.chat_session[session]
    }
    
    if (model) then
        send_data.data.model = model
    else
        send_data.data.model = 'Jiemeng'
    end
    local output = ''
    local sts, err = pcall(function()
        output = bot.request_api(send_data).message.content
        local ta = chat_make_content('assistant', output)
        table.insert(chat.chat_session[session].messages, ta)
    end)
    if not sts then
        output = '咦，脑袋没了！！'
    end
    return bot.string_only(output)
end
--[[
function chat(message)
    local session = ''
    if (message:is_group()) then session = session .. message.group_id .. '_' end
    session = session .. message.user_id
    local para = get_para(message:true_str())
    local rt = _chat(session, para)
    return rt
end]]

function chat.mapi.random_chat(message)
    local session = 'random'
    chat_session[session] = nil
    local rt = _chat(session, message:get_string())
    return rt
end
local function chat_all(message, model)
    local session = ''
    if (message:is_group()) then session = session .. message.group_id .. '_' end
    session = session .. message.user_id
    local para = bot.string.reverse_split(message:true_str())
    local rt = chat.chat(session, para, model)
    return rt
end
function chat.mapi.chat(message)
    return chat_all(message, 'Jiemeng')
end
function chat.mapi.chat32(message)
    return chat_all(message, 'gddisney/llama3.2-uncensored')
end
function chat.mapi.cat(message)
    return chat_all(message, 'Jiemeng_cat')
end
function chat.mapi.chat_jmd(message)
    return chat_all(message, 'Jiemengd')
end
return chat
