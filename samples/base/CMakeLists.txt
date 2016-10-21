cmake_minimum_required(VERSION 3.1)

# determine the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(base)

# define the executable
set(EXECUTABLE_NAME "base")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules")

include("${CMAKE_SOURCE_DIR}/../../cmake/Config.cmake")

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_FORCE_CXX_COMPILER clang)

if(LUG_COMPILER_MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        list(APPEND CMAKE_CXX_FLAGS "/W4")
    endif()
elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
    list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++14")
    string(REPLACE ";" " " "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")
endif()

if(ANDROID_NATIVE_API_LEVEL EQUAL 23)
    set(LUG_STATIC_LIBRARIES true)
endif()


# find Lugdunum and link with it
find_package(LUG REQUIRED system)

include_directories(${LUG_INCLUDE_DIR})

if(LUG_OS_ANDROID)
    add_library(${EXECUTABLE_NAME}
                SHARED
                main.cpp
    )

    target_link_libraries(${EXECUTABLE_NAME}
                          log
                          android
                          ${LUG_LIBRARIES}
    )
else()
    add_executable(${EXECUTABLE_NAME} main.cpp)
    target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
endif()

if(ENABLE_TEST)
    # This adds another subdirectory, which has 'project(gtest)'.
    add_subdirectory(googletest)

    enable_testing()

    # Include the gtest library. gtest_SOURCE_DIR is available due to
    # 'project(gtest)' above.
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    ##############
    # Unit Tests
    ##############
    add_executable(runUnitTests test_exception1.cpp)

    # Standard linking to gtest stuff.
    target_link_libraries(runUnitTests gtest gtest_main)

    # Extra linking for the project.
    target_link_libraries(runUnitTests ${LUG_LIBRARIES})

    # This is so you can do 'make test' to see all your tests run, instead of
    # manually running the executable runUnitTests to see those specific tests.
    add_test(NAME ExcepetionReturnValues COMMAND runUnitTests)
endif()
