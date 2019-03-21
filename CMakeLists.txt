cmake_minimum_required(VERSION 2.8)

project(markup_tool)

include_directories(${CMAKE_SOURCE_DIR}/markup_backend/include/)

add_subdirectory(markup_backend)

add_executable(main main.cpp)
target_link_libraries(main markup_backend)