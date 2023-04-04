set(CMAKE_CXX_STANDARD 20)


PROJECT(Jiemeng)
set(BUILD_SHARED_LIBS OFF)
# list(APPEND SOURCES ${SOURCES_DIR}/Jiemeng.rc)
add_executable(Jiemeng Jiemeng.cpp Jiemeng.rc)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 871ed52d350214a034f6ef8a3b8f51c5ce1bd400) # The commit hash for 1.9.0. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
target_link_libraries(Jiemeng -static-libgcc -static-libstdc++ ws2_32)
target_link_libraries(Jiemeng PRIVATE cpr::cpr)
