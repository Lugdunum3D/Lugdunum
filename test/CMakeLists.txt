cmake_minimum_required(VERSION 3.1)

# project name
project(test)

include("${CMAKE_SOURCE_DIR}/cmake/Config.cmake")

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Threads REQUIRED)

add_executable(runSystemUnitTests ${CMAKE_CURRENT_SOURCE_DIR}/Exception.cpp)
target_link_libraries(runSystemUnitTests gtest gtest_main lug-system ${CMAKE_THREAD_LIBS_INIT})

if(NOT DEFINED TEST_OUTPUT)
    set(TEST_OUTPUT ".")
endif()

add_test(NAME SystemUnitTests COMMAND runSystemUnitTests --gtest_output=xml:${TEST_OUTPUT}/SystemUnitTests.xml)
