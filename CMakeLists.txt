cmake_minimum_required(VERSION 2.8)

project(broadcast)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(./include)

set(src src/common.cpp
        src/main.cpp)

set(basic_link_lib c rt curl event )

add_executable(broadcast ${src})
target_link_libraries(broadcast ${basic_link_lib})
