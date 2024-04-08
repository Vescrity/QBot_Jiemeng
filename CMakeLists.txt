cmake_minimum_required(VERSION 3.10)
# 设置项目名称
project(Jiemeng VERSION 24.99.0)
# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Debug
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
# 设置编译选项，添加-lcurl
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcurl -llua -Wall")
# 设定包含的源文件目录
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code")
# 搜索符合条件的源文件
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
# 过滤掉以test开头的文件
foreach(SOURCE_PATH ${SOURCES})
    get_filename_component(SOURCE_FILE_NAME "${SOURCE_PATH}" NAME)
    if(SOURCE_FILE_NAME MATCHES "^test.*")
        list(REMOVE_ITEM SOURCES "${SOURCE_PATH}")
    endif()
    if(SOURCE_FILE_NAME MATCHES "^ST.*")
        list(REMOVE_ITEM SOURCES "${SOURCE_PATH}")
    endif()
endforeach()
# 指定生成目标
add_executable(Jiemeng ${SOURCES})
# 链接CURL库
target_link_libraries(Jiemeng ${CURL_LIBRARY})
# 指定可执行文件输出的文件夹
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test_runspace")