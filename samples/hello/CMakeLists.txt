cmake_minimum_required(VERSION 3.1)

# determine the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(hello)

# include macros
include(${PROJECT_SOURCE_DIR}/../Macros.cmake)

# define the executable
set(EXECUTABLE_NAME "hello")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules")

include("${CMAKE_SOURCE_DIR}/../../cmake/Config.cmake")

find_package(Vulkan)
if (NOT VULKAN_INCLUDE_DIR)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/../../../thirdparty/vulkan")
        message(FATAL_ERROR "Can't find vulkan, call `git submodule update --recursive`")
    endif()

    set(VULKAN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../../thirdparty/vulkan/include)
endif()

include_directories(${VULKAN_INCLUDE_DIR})

# find Lugdunum and link with it
find_package(LUG REQUIRED core graphics system window math)

include_directories(${LUG_INCLUDE_DIR})
include_directories(include)

set(SHADERS
    shader.vert
    shader-directional.frag
    shader-point.frag
    shader-spot.frag)

if(LUG_OS_ANDROID)
    add_library(${EXECUTABLE_NAME}
                SHARED
                include/Application.hpp
                src/Application.cpp
                src/main.cpp
    )


    target_link_libraries(${EXECUTABLE_NAME}
                          log
                          android
                          ${LUG_LIBRARIES}
    )

    target_compile_options(${EXECUTABLE_NAME} PUBLIC -std=c++14)
else()
    add_executable(${EXECUTABLE_NAME} include/Application.hpp src/Application.cpp src/main.cpp)
    compile_shaders(${EXECUTABLE_NAME} ${SHADERS})
    target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
endif()

# install models
file(COPY ${LUG_RESOURCES_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# TODO: Use macro to generate target and add compilation flags
