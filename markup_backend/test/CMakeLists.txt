# https://github.com/albertchan/cmake-boost-demo

# Project settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")

# Dependencies
find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)

set(PROJECT_UNIT_TESTS_NAME test_backend)

# Gather the header and source files
file(GLOB UNIT_TESTS_INC_FILES ${PROJECT_SOURCE_DIR}/markup_backend/test/*.hpp)
file(GLOB UNIT_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/markup_backend/test/*.cpp)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/markup_backend/include)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/markup_backend/src/*.cpp)

# Include paths
set(UNIT_TESTS_INCLUDES ${PROJECT_INCLUDE_DIR}
        # ${PROJECT_SOURCE_DIR}/backend/src
        ${Boost_INCLUDE_DIR})

# Assign the include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${UNIT_TESTS_INCLUDES})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Build unit tests
add_definitions(-DBOOST_TEST_DYN_LINK)
add_executable(${PROJECT_UNIT_TESTS_NAME} ${UNIT_TESTS_SRC_FILES} ${SRC_FILES})  #  ${SRC_FILES}

target_link_libraries(${PROJECT_UNIT_TESTS_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS})

enable_testing()
add_test(TestBackend ${PROJECT_UNIT_TESTS_NAME})