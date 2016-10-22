cmake_minimum_required(VERSION 3.1)

# determine the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# project name
project(base)

# define the executable
set(EXECUTABLE_NAME "base")
add_executable(${EXECUTABLE_NAME} main.cpp)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules")

# find Lugdunum and link with it
find_package(LUG REQUIRED system)
if(LUG_FOUND)
    include_directories(${LUG_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
endif()
