cmake_minimum_required(VERSION 3.1)

# project name
project(test)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

# Find gmock
find_package(GMock)
if (NOT GMOCK_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/googlemock")
        message(FATAL_ERROR "Can't find googlemock in the thirdparty directory")
    endif()

    set(GMOCK_ROOT "${LUG_THIRDPARTY_DIR}/googlemock")

    if (LUG_OS_WINDOWS)
        set(GMOCK_MSVC_SEARCH "MT")
    endif()

    find_package(GMock REQUIRED)
endif()

# Find gtest
find_package(GTest)
if (NOT GTEST_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/googlemock")
        message(FATAL_ERROR "Can't find googlemock in the thirdparty directory")
    endif()

    set(GTEST_ROOT "${LUG_THIRDPARTY_DIR}/googlemock")

    if (LUG_OS_WINDOWS)
        set(GTEST_MSVC_SEARCH "MT")
    endif()

    find_package(GTest REQUIRED)
endif()

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

find_package(Threads REQUIRED)

if(NOT DEFINED TEST_OUTPUT)
    set(TEST_OUTPUT ".")
endif()

add_subdirectory(System)
add_subdirectory(Math)
add_subdirectory(Graphics)
