cmake_minimum_required(VERSION 3.27)
project(WinsockServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(WinsockServer main.cpp)

if(MINGW)
    target_link_libraries(WinsockServer ws2_32)
endif()


