--[[
function _cpp_core(cpp_code)
    local executable_file_name = os.tmpname()
    local temp_file_name = executable_file_name .. ".cpp"
    -- local executable_file_name = os.tmpname()
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(cpp_code)
    temp_file:close()
    local compile_command = string.format("g++ -o %s %s 2>&1",
                                          executable_file_name, temp_file_name)
    local compile_handle = io.popen(compile_command, "r")
    local compile_output = compile_handle:read("*a")
    local compile_success = compile_handle:close()
    if compile_success then
        local execute_command = executable_file_name
        local execute_handle = io.popen(execute_command, "r")
        local execute_output = execute_handle:read("*a")
        execute_handle:close()
        os.remove(temp_file_name)
        os.remove(executable_file_name)
        return execute_output
    else
        os.remove(temp_file_name)
        os.remove(executable_file_name)
        return compile_output 
    end
end

function _cpp_run(code)
    local pre = [==[
    #include<stdio.h>
    #include<stdlib.h>
    #include<thread>
    #include <unistd.h>
    void calc();
    void tm(){
      usleep(200000);
      printf("\nTLE\n");
      exit(1);
    }
    int main(){
      std::thread t(tm);
      t.detach();
      calc();
      return 0;
    }
    ]==] 
    return _cpp_core(pre .. code)
end
]]


function _cpp_run(code)
    local executable_file_name = os.tmpname()
    local tmpscript_file_name = os.tmpname()
    local temp_file_name = executable_file_name .. ".cpp"
    -- local executable_file_name = os.tmpname()
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(code)
    temp_file:close()
    local temp_file = assert(io.open(tmpscript_file_name, "w"))
    local script= 'g++ ' .. temp_file_name .. ' -o /tmp/qwq && /tmp/qwq' 
    temp_file:write(script)
    temp_file:close()
    local rt=bot.os_sh('./wrap -GC --ro-bind '.. temp_file_name  
        .. ' ' .. temp_file_name 
        .. ' --ro-bind ' .. tmpscript_file_name .. ' ' .. tmpscript_file_name
        .. ' sh ' .. tmpscript_file_name)
    os.remove(temp_file_name)
    os.remove(tmpscript_file_name)
    os.remove(executable_file_name)
    return bot.string_only(rt)
end

function cpp_run(message)
    local para = get_para(message:true_str())
    return _cpp_run(para)
end

function _lua_run(lua_code)
    local executable_file_name = os.tmpname()
    local temp_file_name = executable_file_name .. ".lua"
    -- local executable_file_name = os.tmpname()
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(lua_code)
    temp_file:close()
    local rt=bot.os_sh('./wrap --ro-bind '.. temp_file_name .. 
        ' ' .. temp_file_name .. ' /usr/bin/lua ' .. temp_file_name)
    os.remove(temp_file_name)
    os.remove(executable_file_name)
    return bot.string_only(rt)
end
function _py_run(code)
    local executable_file_name = os.tmpname()
    local temp_file_name = executable_file_name .. ".py"
    -- local executable_file_name = os.tmpname()
    local temp_file = assert(io.open(temp_file_name, "w"))
    temp_file:write(code)
    temp_file:close()
    local rt=bot.os_sh('./wrap --ro-bind '.. temp_file_name .. 
        ' ' .. temp_file_name .. ' /usr/bin/python ' .. temp_file_name)
    os.remove(temp_file_name)
    os.remove(executable_file_name)
    return bot.string_only(rt)
end
function lua_run(message)
    local para = get_para(message:true_str())
    return _lua_run(para)
end
function py_run(message)
    local para = get_para(message:true_str())
    return _py_run(para)
end
function wrap_shell(message)
    local para = get_para(message:true_str())
    return _lua_run([=[os.execute([==[]=] .. para .. [=[]==])]=])
end
