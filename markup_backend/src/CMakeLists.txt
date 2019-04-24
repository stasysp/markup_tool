

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/markup_backend/include)
include_directories(${HEADERS_DIR})

find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# set(HEADERS
#         ${HEADERS_DIR}/markup_backend/video.h
#         ${HEADERS_DIR}/markup_backend/markup.h)

add_library(markup_backend markup.cpp video.cpp tracks.cpp utils.cpp)
target_link_libraries(markup_backend ${OpenCV_LIBS} ${Boost_LIBRARIES})