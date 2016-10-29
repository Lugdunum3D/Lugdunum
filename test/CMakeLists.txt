cmake_minimum_required(VERSION 2.8)

# project name
project(test)

include("${CMAKE_SOURCE_DIR}/cmake/Config.cmake")

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_FORCE_CXX_COMPILER clang)

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Threads REQUIRED)

add_executable(runSystemUnitTests ${CMAKE_CURRENT_SOURCE_DIR}/exception.cpp)
target_link_libraries(runSystemUnitTests gtest gtest_main lug-system ${CMAKE_THREAD_LIBS_INIT})

add_test(allTests runSystemUnitTests)
