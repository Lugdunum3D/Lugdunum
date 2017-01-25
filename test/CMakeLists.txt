cmake_minimum_required(VERSION 3.1)

# project name
project(test)

include("${CMAKE_SOURCE_DIR}/cmake/Config.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/Macros.cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY=1)

find_package(Threads REQUIRED)

if(NOT DEFINED TEST_OUTPUT)
    set(TEST_OUTPUT ".")
endif()

add_subdirectory(System)
add_subdirectory(Math)
