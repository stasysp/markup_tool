cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(markup_tool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")

include_directories(${CMAKE_SOURCE_DIR}/markup_backend/include/)

add_subdirectory(markup_backend)

add_executable(main main.cpp)
target_link_libraries(main markup_backend)