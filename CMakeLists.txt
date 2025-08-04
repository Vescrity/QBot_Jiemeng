cmake_minimum_required(VERSION 3.17)
project(Jiemeng)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall)

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ..
    COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building Debug version"
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code")
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

foreach(SOURCE_PATH ${SOURCES})
    get_filename_component(SOURCE_FILE_NAME "${SOURCE_PATH}" NAME)
    if(SOURCE_FILE_NAME MATCHES "^test.*")
        list(REMOVE_ITEM SOURCES "${SOURCE_PATH}")
    endif()
endforeach()

add_executable(Jiemeng ${SOURCES})

target_include_directories(Jiemeng PRIVATE ${LUA_INCLUDE_DIRS})
target_link_libraries(Jiemeng PRIVATE ${LUA_LIBRARIES} curl)

