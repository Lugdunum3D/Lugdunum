cmake_minimum_required(VERSION 2.8)

# project name
project(test)

include("${CMAKE_SOURCE_DIR}/cmake/Config.cmake")

if(LUG_COMPILER_MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        list(APPEND CMAKE_CXX_FLAGS "/W4")
    endif()
elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
    list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
    string(REPLACE ";" " " "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")
endif()

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Threads REQUIRED)

add_executable(runSystemUnitTests ${CMAKE_CURRENT_SOURCE_DIR}/Exception.cpp)
target_link_libraries(runSystemUnitTests gtest gtest_main lug-system ${CMAKE_THREAD_LIBS_INIT})

if(NOT DEFINED TEST_OUTPUT)
    set(TEST_OUTPUT ".")
endif()

add_test(NAME SystemUnitTests COMMAND runSystemUnitTests --gtest_output=xml:${TEST_OUTPUT}/SystemUnitTests.xml)
