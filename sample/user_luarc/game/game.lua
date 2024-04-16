Character = {}
Character.__index = Character
-- 定义Character的构造函数
function Character:new(character, name, level, hp, atk, def, mdef)
    local default = {
        name = "无名氏",
        hp = 0.0,
        atk = 0.0,
        def = 0.0,
        mdef = 0.0,
        level = 0,
        stage = 1
    }
    local obj
    -- if character == nil then
    obj = setmetatable({}, Character)
    -- else
    --    obj = setmetatable({}, {__index = character})
    -- end
    obj.name = name or character and character.name or default.name
    obj.level = level or character and character.level or default.level
    obj.hp = hp or character and character.hp or default.hp
    obj.atk = atk or character and character.atk or default.atk
    obj.def = def or character and character.def or default.def
    obj.mdef = mdef or character and character.mdef or default.mdef
    obj.stage = character and character.stage or default.stage
    return obj
end

function Character:clear()
    self.name = "无名氏"
    self.hp = 0.0
    self.atk = 0.0
    self.def = 0.0
    self.mdef = 0.0
    self.level = 0
    self.stage = 1
end

function Character:status()
    local rt = ''
    rt = rt .. ("姓名: " .. self.name) .. '\n'
    rt = rt .. ("等级: " .. self.level) .. '\n'
    rt = rt .. ("HP: " .. self.hp) .. '\n'
    rt = rt .. ("ATK: " .. self.atk) .. '\n'
    rt = rt .. ("DEF: " .. self.def) .. '\n'
    rt = rt .. ("MDEF: " .. self.mdef)
    return rt
end

function Character:level_up(select)
    select = tonumber(select)
    up = math.min(self.atk, self.def, self.mdef)
    if (select == 1) then
        self.atk = self.atk + up
    elseif (select == 2) then
        self.def = self.def + up * 1.25
    elseif (select == 3) then
        self.mdef = self.mdef + up * 1.5
    else
        return '无效的参数'
    end
    self.hp = self.hp + up * 4.0
    self.hp = self.hp * 2.0
    self.level = self.level + 1
    self.atk = self.atk * 1.24
    self.def = self.def * 1.24
    self.mdef = self.mdef * 1.24
    return '升级成功'
end

function pk(a, b)
    if (a.def >= b.atk and b.def >= a.atk) then
        a.hp = 0.1
        return
    end
    local harm_a = b.atk - a.def
    local harm_b = a.atk - b.def
    if (harm_a <= 0) then harm_a = 0 end
    if (harm_b <= 0) then harm_b = 0 end
    if (harm_b == 0) then
        a.hp = 0
        return
    end
    if (harm_a == 0) then return end
    local na = (a.hp - 0.001) // harm_a + 1
    local nb = (b.hp - 0.001) // harm_b + 1
    if (nb > na) then
        a.hp = 0
        return
    end
    a.hp = a.hp - (nb - 1) * harm_a
    return
end

function challenge(player, mon)
    local tryplay = Character:new(player)
    tryplay.hp = player.hp + player.mdef
    pk(tryplay, mon)
    if (tryplay.hp > 0) then -- 战胜
        if (tryplay.hp < player.hp) then player.hp = tryplay.hp end
        return true
    else -- 战败
        return false
    end
end

function gen_monster(level)
    local mon = Character:new(nil, '怪物' .. tostring(level), level, 5, 1.6,
                              1.3, 0)
    local times = (1.645 ^ (level - 1))
    mon.hp = mon.hp * times
    mon.def = mon.def * times
    mon.atk = mon.atk * times
    mon.mdef = mon.mdef * times
    return mon
end

function new_player(name)
    local player = Character:new(nil, name, 1, 50, 2, 1, 2)
    player.stage = 1
    return player
end

function save_game_data(file_path, game_data)
    -- 把Lua table转换为JSON对象
    local json_obj = jsonlib.table2json(game_data)
    -- 转换为字符串格式，并确保进行缩进以便于阅读
    local json_str = json_obj:dump()
    -- 打开文件用于写入
    local file = io.open(file_path, "w")
    if not file then return false, "无法打开文件进行写入" end
    -- 写入数据到文件
    file:write(json_str)
    -- 关闭文件句柄
    file:close()
    return true, '成功写入'
end
game_data = {}
function load_game_data(file_path)
    -- 打开文件用于读取
    local file = io.open(file_path, "r")
    if not file then return "无法打开文件进行读取" end
    -- 读取文件内容
    local json_str = file:read("*a")
    -- 关闭文件句柄
    file:close()
    -- 解析JSON字符串为JSON对象
    local json_obj = json:new()
    json_obj:parse(json_str)
    if not json_obj then return "JSON解析失败" end
    -- 获取table格式的实际值
    local gdata = json_obj:val()
    game_data = {}
    for k, v in pairs(gdata) do game_data[k] = Character:new(v) end
    return '加载成功'
end

function Character:_game(oper, oper2)
    if (oper == 'status') then
        return self:status()
    elseif (oper == 'challenge') then
        if (self.stage ~= self.level) then return '需要升级' end
        local pass = challenge(self, gen_monster(self.stage))
        if (pass) then
            self.stage = self.stage + 1
            if (oper2) then self:level_up(oper2) end
            return '挑战成功\n' .. self:status()
        else
            return '挑战失败'
        end
    elseif (oper == 'up') then
        if (oper2 == nil) then return '缺少第二参数' end
        if (self.level >= self.stage) then return '不满足升级条件' end
        return self:level_up(oper2) .. '\n' .. self:status()
    elseif (oper == 'newgame') then
        self:clear()
        self.name = oper2
        return '角色重置成功'
    elseif (oper == 'mon_info') then
        local mon = gen_monster(self.stage)
        return mon:status()
    elseif (oper == 'save') then
        local a, r = save_game_data('./game_data.json', game_data)
        return r
    elseif (oper == 'load') then
        local r = ''
        r = load_game_data('./game_data.json')
        return r
    elseif (oper == 'info') then
        local r = ''
        r = r .. [====[
伤害计算：
    玩家先手，产生伤害为 己方ATK-敌方DEF
    MDEF 在每局战斗中可抵掉等量的伤害
    若战斗双方均不能对对方造成伤害则将血量清至 0.1 并结束本局战斗。
升级机制：
    每次战斗胜利后进行一次升级
    当前升级逻辑：
        记 up = min(ATK DEF MDEF)
        若升级 ATK，则加 up*1.0
        若升级 DEF，则加 up*1.25
        若升级 MDEF，则加 up*1.5
        随后生命加 up*4.0，随后 *2.0
        其余属性 *1.24
怪物属性：
    初始值为 5 1.6 1.3 0
    随后每升级一次全属性 *1.645]====]
        return r
    else
        return
            '操作：\nstatus 状态信息\nmon_info 怪物信息\nchallenge[#1/2/3] 挑战[并升级]\nup#[1/2/3] 升级 ATK/DEF/MDEF\nnewgame[#name] 新游戏[名称]'
    end
end

function game(message)
    local player = game_data[message.place.user_id] or
                       new_player(message.place.user_nm)
    if (player.level == 0) then player = new_player(player.name) end
    local ord = get_para(message:true_str())
    local op2, op1 = get_para(ord)
    local rt = player:_game(op1, op2)
    if (op1 ~= 'load') then
        game_data[message.place.user_id] = Character:new(player)
    end
    return rt
end

