include_directories(${CMAKE_SOURCE_DIR}/markup_backend/include/)
message(${CMAKE_SOURCE_DIR}/markup_backend/include/)

add_library(markup_backend pipeline.cpp)