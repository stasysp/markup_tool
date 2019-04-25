cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(markup_tool)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")  #  -stdlib=libc++
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt5.5.0/5.5/gcc_64)

include_directories(${CMAKE_SOURCE_DIR}/markup_backend/include/)
include_directories(${CMAKE_SOURCE_DIR}/markup_frontend/include/)

add_subdirectory(markup_backend)
add_subdirectory(markup_frontend)

add_dependencies(markup_frontend markup_backend)

add_executable(main main.cpp)
target_link_libraries(main markup_backend markup_frontend)
