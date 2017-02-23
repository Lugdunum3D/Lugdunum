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

# find Lugdunum and link with it
find_package(LUG REQUIRED window system)

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

    target_compile_options(${EXECUTABLE_NAME} PUBLIC -std=c++14)
else()
    add_executable(${EXECUTABLE_NAME} main.cpp)
    target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
endif()

# TODO: Use macro to generate target and add compilation flags
