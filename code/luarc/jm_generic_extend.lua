---@module 'LuaMetaLib'
local ex = {}
ex.algo = require("jm_algorithm")
---@class GenericCache
---@field path string
---@field payload table
ex.Cache = {}
ex.Cache.__index = ex.Cache
---comment
---@param path string
---@return GenericCache
function ex.Cache.new(path)
    local i = { init = false, payload = {}, path = path }
    setmetatable(i, ex.Cache)
    return i
end

function ex.Cache:reload()
    self.init = false
end

function ex.Cache:get()
    if not self.init then
        self.payload = ex.file2table(self.path)
        self.init = true
    end
    return self.payload
end

function ex.Cache:save()
    ex.table2file(self.payload, self.path)
end

---@type fun(t: table): string
ex.table2str = require("cjson.util").serialise_value
---save a table to file
---@param t table
---@param file_name string
function ex.table2file(t, file_name)
    local str = 'return ' .. ex.table2str(t)
    local file = assert(
        io.open(file_name, "w")
    )
    file:write(str)
    file:close()
end

---**DOFILE**, expect it as a table
---@param file_name string
---@return table
function ex.file2table(file_name)
    local ok, t = pcall(dofile, file_name) -- 安全加载文件
    if not ok or type(t) ~= 'table' then return {} end
    return t
end

---@class random_item
---@field weight integer
---@field payload any

---@alias random_list random_item[]

---@param obj_list random_list
---@return any
function ex.get_random_obj(obj_list)
    local sum = 0
    for _, v in ipairs(obj_list) do
        sum = sum + v.weight
    end
    local r = bot.rand(1, sum)
    for _, v in ipairs(obj_list) do
        if r <= v.weight then
            return v.payload
        else
            r = r - v.weight
        end
    end
end

ex.string = {}
---
---@param s string
---@param spliter? string
---@return string tail
---@return string head
function ex.string.reverse_split(s, spliter)
    spliter = spliter or (bot and bot.spliter) or '#'
    local pos = s:find(spliter)
    if pos then
        return s:sub(pos + #spliter), s:sub(1, pos - 1)
    else
        return '', s
    end
end

---包装任意文本的字符串用于命令行参数传递
---@param arg string
---@return string
function ex.string.qoute(arg)
    return "'" .. arg:gsub("'", "'\\''") .. "'"
end

---简易命令解析
---@param options_table table
---@param options_string string
---@return table
function ex.string.parse_options(options_table, options_string)
    local result = {}
    -- options_string = options_string:gsub("\r", "")
    -- 分割字符串为行
    local lines = {}
    for line in options_string:gmatch("([^\n]+)") do
        table.insert(lines, line)
    end
    for _, line in ipairs(lines) do
        for key, flag in pairs(options_table) do
            local pattern = "^" .. flag .. "%s+(.+)$"
            local match = line:match(pattern)
            if match then
                -- 去除匹配到字符串两端的空格
                result[key] = match:match("^%s*(.-)%s*$")
            end
        end
    end
    -- 对未找到的选项赋nil
    for key, _ in pairs(options_table) do
        if not result[key] then result[key] = nil end
    end
    return result
end

---replace `{{value}}` to t.value
---@param template string
---@param t {[string]: string|number}
---@return string
function ex.string.format_from_table(template, t)
    for k, v in pairs(t) do
        print('extend: ', k, v)
        template = template:gsub("{{" .. k .. "}}", function() return tostring(v) end)
    end
    return template
end

ex.file = {}
function ex.file.read(filename)
    local f = io.open(filename, "r")
    assert(f ~= nil)
    local s = f:read("*a")
    f:close()
    return s
end

function ex.file.write(filename, str)
    local f = io.open(filename, "w")
    if (f == nil) then return false end
    f:write(str)
    f:close()
    return true
end

return ex
