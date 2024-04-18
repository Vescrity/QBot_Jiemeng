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
        return compile_output -- 返回编译错误的信息
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

function cpp_run(message)
    local para = get_para(message:true_str())
    return _cpp_run(para)
end
