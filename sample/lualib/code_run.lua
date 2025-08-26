mapi = mapi or {}
require('bot_string')
local reverse_split = require("luarc.jm_generic_extend").string.reverse_split
---
---@param code string
---@return string
local function cpp_run(code)
    local executable_file_name = os.tmpname()
    local tmpscript_file_name = os.tmpname()
    local temp_file_name = executable_file_name .. ".cpp"
    -- local executable_file_name = os.tmpname()
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(code)
    temp_file:close()
    local temp_file = assert(io.open(tmpscript_file_name, "w"))
    local script = 'g++ ' .. temp_file_name .. ' -o /tmp/qwq && /tmp/qwq'
    temp_file:write(script)
    temp_file:close()
    local rt = bot.os_sh('./wrap -GC --ro-bind ' .. temp_file_name
        .. ' ' .. temp_file_name
        .. ' --ro-bind ' .. tmpscript_file_name .. ' ' .. tmpscript_file_name
        .. ' sh ' .. tmpscript_file_name)
    os.remove(temp_file_name)
    os.remove(tmpscript_file_name)
    os.remove(executable_file_name)
    return bot.string_only(rt)
end

---comment
---@param message Message
---@return string
function mapi.cpp_run(message)
    local para = reverse_split(message:true_str())
    return cpp_run(para)
end

---comment
---@param lua_code string
---@param jit boolean|nil
---@return string
local function lua_run(lua_code, jit)
    local executable_file_name = os.tmpname()
    local LUA = jit and '/usr/bin/luajit' or '/usr/bin/lua'
    local temp_file_name = executable_file_name .. ".lua"
    -- local executable_file_name = os.tmpname()
    local RUN = string.format(
        './wrap --ro-bind %s %s %s %s',
        temp_file_name,
        temp_file_name,
        LUA,
        temp_file_name
    )
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(lua_code)
    temp_file:close()
    local rt = bot.os_sh(RUN)
    os.remove(temp_file_name)
    os.remove(executable_file_name)
    return bot.string_only(rt)
end

---comment
---@param code string
---@return string
local function py_run(code)
    local executable_file_name = os.tmpname()
    local temp_file_name = executable_file_name .. ".py"
    -- local executable_file_name = os.tmpname()
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(code)
    temp_file:close()
    local rt = bot.os_sh('./wrap --ro-bind ' .. temp_file_name ..
        ' ' .. temp_file_name .. ' /usr/bin/python ' .. temp_file_name)
    os.remove(temp_file_name)
    os.remove(executable_file_name)
    return bot.string_only(rt)
end

---comment
---@param message Message
---@return string
function mapi.lua_run(message)
    local para = reverse_split(message:true_str())
    return lua_run(para)
end

---comment
---@param message Message
---@return string
function mapi.luajit_run(message)
    local para = reverse_split(message:true_str())
    return lua_run(para, true)
end

---comment
---@param message Message
---@return string
function mapi.py_run(message)
    local para = reverse_split(message:true_str())
    return py_run(para)
end

---comment
---@param message Message
---@return string
function mapi.wrap_shell(message)
    local para = reverse_split(message:true_str())
    return lua_run([=[os.execute([==[]=] .. para .. [=[]==])]=])
end

return {
    cpp_run,
    lua_run,
    py_run
}
