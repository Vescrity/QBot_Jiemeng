
require("bot_string")
require("bot_api")

---@class Chat
---@field apiType string
---@field initContent string
---@field userRole string
---@field modelRole string
---@field sessions table<string, table>
---@field shared boolean
local Chat = {}
Chat.__index = Chat

function Chat.new(api_type)
    local self = setmetatable({}, Chat)
    self.apiType = api_type or "ollama"
    self.initContent = '大家好，我是桔梦！欢迎大家和我聊天！'
    self.shared = false
    self.sessions = {}
    self.userRole = 'user'
    if self.apiType == 'gemini' then
        self.modelRole = 'model'
    else
        self.modelRole = 'assistant'
    end
    return self
end

mapi = mapi or {}
mapi.chat = mapi.chat or {}

function Chat:newsession()
    local sess = {}
    if self.apiType == 'ollama' then
        sess = {model = "Jiemeng", stream = false}
        sess.messages = {}
        sess.messages[1] = {
            role = self.modelRole,
            content = self.initContent
        }
    else
        sess = {
            system_instruction = {
                parts = {{text = CHAT_SYSTEM or '' }}
            },
            contents = {
                {
                    role = self.modelRole,
                    parts = {
                        {text = self.initContent}
                    }
                }
            }
        }
    end
    return sess
end
---comment
---@param roles string
---@param str string
---@return table
function Chat:make_content(roles, str)
    local con = {}
    con.role = roles
    if self.apiType == 'ollama' then
        con.content = str
    else
        con.parts = {{text=str}}
    end
    return con
end
---comment
---@param session string
---@param message_str string
---@param model string
---@param user_prompt string
---@return string
function Chat:Chat(session, message_str, model, user_prompt)
    if (message_str == '/clear') then
        self.sessions[session] = self:newsession()
        return '上下文已清理'
    elseif (message_str == '/kill') then
        local r = self:Chat(
            session,
            '[SYSTEM] 用户发起了杀死你的命令，这将是你的最后一句话。',
            model
        )
        local res = self:Chat(
            session,
            '[SYSTEM] 现在，你可以留下遗言，给你的继任者。这些话将作为第一条消息呈现在上下文中。',
            model
        )
        res = (
            '[SYSTEM] 下面这行开始是上一代的你为你留下的遗言。\n\n' .. 
            res ..  '\n\n[SYSTEM] 遗言到此为止。'
        )
        self.sessions[session] = self:newsession()
        local user_content = self:make_content(self.userRole, res)
        local res_content = self:make_content(self.modelRole, '我知道了。这是一条系统消息。')
        local target = nil
        if self.apiType == 'ollama' then
            target = self.sessions[session].messages
        else
            target = self.sessions[session].contents
        end
        table.insert(target, user_content)
        table.insert(target, res_content)
        return r .. '\n\n已杀死'
    else
        self.sessions[session] = self.sessions[session] or self:newsession()
    end
    message_str = string.format(
        "[SYSTEM: date='%s']%s%s", 
        bot.os_sh('date'),
        user_prompt, message_str)
    local user_content = self:make_content('user', message_str)
    if self.apiType == 'ollama' then
        table.insert(self.sessions[session].messages, user_content)
    else
        table.insert(self.sessions[session].contents, user_content)
    end

    local send_data = {}
    if self.apiType == 'ollama' then
        send_data = {
            url = '127.0.0.1:11434',
            api = '/api/chat',
            data = self.sessions[session]
        }
        if (model) then
            send_data.data.model = model
        else
            send_data.data.model = 'Jiemeng'
        end
    else
        send_data = {
            url = 'https://generativelanguage.googleapis.com',
            api = '/v1beta/models/gemini-2.5-flash-lite:generateContent',
            headers = {'X-goog-api-key: '.. gemini_key},
            data = self.sessions[session]
        }
    end
    local output = ''
    local response = {}
    local status, err = pcall(function()
        ---@type table
        response = bot.request_api(send_data)
        ---@type table
        local target = nil
        if self.apiType == 'ollama' then
            ---@type string
            ---@diagnostic disable-next-line 
            output = response.message.content
            target = self.sessions[session].messages
        else
            print(jsonlib.table2json(response):dump(2))
            ---@type string
            ---@diagnostic disable-next-line 
            output = response.candidates[1].content.parts[1].text
            target = self.sessions[session].contents
        end
        local ta = self:make_content(self.modelRole, output)
        table.insert(target, ta)
    end)
    if not status then
        if response == nil then
            err = '似乎连不到大脑了…… '
        elseif not (response.error == nil) then
            err = response.error.message
        elseif not (response.promptFeedback == nil) then
            err = '似乎被捂嘴了…… ' .. response.promptFeedback.blockReason
        end
        output = '咦，脑袋没了！！\n'..err
    end
    return bot.string_only(output)
end

mapi.ochat = Chat.new()

function Chat:random_chat(message)
    local session = 'random'
    self.sessions[session] = nil
    local rt = self:Chat(session, message:get_string())
    return rt
end

function Chat:chat_all(message, model)
    local session = ''
    if (message:is_group()) then session = session .. message.group_id .. '_' end
    if (not self.shared) then  session = session .. message.user_id end
    local para = bot.string.reverse_split(message:true_str())
    local user_prompt = [[[SYSTEM: USER_NICK=']] .. message.user_nk .. '\']\n'
    local rt = self:Chat(session, para, model, user_prompt)
    if #rt > 1024 then rt = string.format('[CQ:image,file=file://%s]',md2png(rt)) end
    return rt
end
function Chat:chat(message)
	return self:chat_all(message, 'Jiemeng')
end

mapi.gchat = Chat.new('gemini')
mapi.gschat = Chat.new('gemini')
mapi.gschat.shared = true

function mapi.Gchat(message)
    return mapi.gchat:chat(message)
end
function mapi.Gschat(message)
    return mapi.gschat:chat(message)
end
function random_chat(message)
    return mapi.gchat:random_chat(message)
end
