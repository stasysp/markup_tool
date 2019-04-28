#cmake_minimum_required(VERSION 2.8)
cmake_minimum_required(VERSION 3.5)
# cmake_minimum_required(VERSION 3.14.0)

# set(CMAKE_C_COMPILER "gcc-4.9")
# set(CMAKE_CXX_COMPILER "g++-4.9")

# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

project(markup_tool)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")  #  -stdlib=libc++
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")
#set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt5.5.0/5.5/gcc_64)

include_directories(${CMAKE_SOURCE_DIR}/markup_backend/include/)
include_directories(${CMAKE_SOURCE_DIR}/markup_frontend/include/)

add_subdirectory(markup_backend)
add_subdirectory(markup_frontend)

add_dependencies(markup_frontend markup_backend)

find_package(Qt5Widgets)

add_executable(main main.cpp)
target_link_libraries(main markup_backend markup_frontend)
