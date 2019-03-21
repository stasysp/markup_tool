include_directories(${CMAKE_SOURCE_DIR}/markup_backend/include/)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(markup_backend markup.cpp)
target_link_libraries(markup_backend ${OpenCV_LIBS})