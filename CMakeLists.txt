cmake_minimum_required(VERSION 3.17)
project(Jiemeng)
add_compile_options(-Wall)
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ..
    COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building Debug version"
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lcurl -llua")

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
endforeach()
# 指定生成目标
add_executable(Jiemeng ${SOURCES})

