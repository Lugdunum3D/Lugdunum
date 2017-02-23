cmake_minimum_required(VERSION 3.1)

# project name
project(test)

include("${CMAKE_SOURCE_DIR}/cmake/Macros.cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

# Find gmock
find_package(GMock)
if (NOT GMOCK_FOUND)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/googlemock")
        message(FATAL_ERROR "Can't find googlemock, call `git submodule update --recursive`")
    endif()

    if (LUG_OS_WINDOWS)
        set(GMOCK_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/googlemock/lib/windows/${ARCH_DIR}")
        set(GMOCK_MSVC_SEARCH "MT")
    elseif (LUG_OS_LINUX)
        set(GMOCK_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/googlemock/lib/linux")
    else ()
        set(GMOCK_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/googlemock/lib/android")
    endif()

    set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/googlemock/include")
    find_package(GMock REQUIRED)
endif()

# Find gtest
find_package(GTest)
if (NOT GTEST_FOUND)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/googletest")
        message(FATAL_ERROR "Can't find googletest, call `git submodule update --recursive`")
    endif()

    if (LUG_OS_WINDOWS)
        set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/googletest/lib/windows/${ARCH_DIR}")
        set(GTEST_MSVC_SEARCH "MT")
    elseif (LUG_OS_LINUX)
        set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/googletest/lib/linux")
    else ()
        set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/googletest/lib/android")
    endif()

    set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/googletest/include")
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
