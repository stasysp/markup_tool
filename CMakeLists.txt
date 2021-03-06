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

set(Qt5_CMAKE_DIR /usr/local/opt/qt/lib/cmake)
set(Qt5Widgets_DIR ${Qt5_CMAKE_DIR}/Qt5Widgets)
find_package(Qt5Widgets)

add_subdirectory(markup_backend)
add_subdirectory(markup_frontend)

add_dependencies(markup_frontend markup_backend)



add_executable(markup_tool main.cpp)
target_link_libraries(markup_tool markup_backend markup_frontend)

file(COPY python DESTINATION .)
file(COPY data DESTINATION .)

set(YOLO_WEIGHTS data/models/yolo3/yolov3.weights)
if (NOT EXISTS ${YOLO_WEIGHTS})
    message("Downloading YOLOv3 weights")
    file(DOWNLOAD
        https://pjreddie.com/media/files/yolov3.weights
        ${YOLO_WEIGHTS})
endif()
