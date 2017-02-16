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
include_directories(${VULKAN_INCLUDE_DIR})

if(LUG_COMPILER_MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        list(APPEND CMAKE_CXX_FLAGS "/W4")
    endif()

    list(APPEND CMAKE_CXX_FLAGS "/WX")
elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
    list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++14")
endif()

string(REPLACE ";" " " "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")

if(ANDROID_NATIVE_API_LEVEL EQUAL 23)
    set(LUG_STATIC_LIBRARIES true)
endif()

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
                main.cpp
    )

    compile_shaders(${EXECUTABLE_NAME} ${SHADERS})
    target_link_libraries(${EXECUTABLE_NAME}
                          log
                          android
                          ${LUG_LIBRARIES}
    )
else()
    add_executable(${EXECUTABLE_NAME} include/Application.hpp src/Application.cpp src/main.cpp)
    compile_shaders(${EXECUTABLE_NAME} ${SHADERS})
    target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
endif()

# install models
file(COPY ${LUG_RESOURCES_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
