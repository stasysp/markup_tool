set(CMAKE_AUTOMOC ON)

# set(Qt5_CMAKE_DIR /usr/local/opt/qt/lib/cmake)
# set(Qt5Widgets_DIR ${Qt5_CMAKE_DIR}/Qt5Widgets)

# include_directories(/usr/local/opt/qt/include)
# set(Qt5Widgets_DIR /usr/local/opt/qt/lib/cmake/Qt5Widgets)

# Backend vvvvvvvvv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# ^^^^^^^^^^^^^^^^^

find_package (Qt5Widgets REQUIRED)
# find_package (Qt5Declarative)

set(FRONTEND_HEADERS_DIR ${CMAKE_SOURCE_DIR}/markup_frontend/include)
include_directories(${FRONTEND_HEADERS_DIR})

include_directories(${Qt5Widgets_INCLUDE_DIRS})
# include_directories(${Qt5Declarative_INCLUDE_DIRS})

file(GLOB FRONTEND_HEADERS ${FRONTEND_HEADERS_DIR}/markup_frontend/*.h)

set(BACKEND_HEADERS_DIR ${CMAKE_SOURCE_DIR}/markup_backend/include)
include_directories(${BACKEND_HEADERS_DIR})
# link_directories(${CMAKE_SOURCE_DIR}/markup_backend/src)

add_library(markup_frontend
        ${FRONTEND_HEADERS}
        framecontrol.cpp
        framewithcontrol.cpp
        framemarkup.cpp
        frameview.cpp
        maincontrolpanel.cpp
        markupwidget.cpp
        scaledbox.cpp
        timelinecontrol.cpp
        application.cpp
        )


target_link_libraries(markup_frontend markup_backend ${Qt5Widgets_LIBRARIES})
