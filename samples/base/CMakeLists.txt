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

if(LUG_COMPILER_MSVC)
   if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
       string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   else()
       list(APPEND CMAKE_CXX_FLAGS "/W4")
   endif()
elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
   list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra")
    if(LUG_OS_ANDROID)
        list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
        string(REPLACE ";" " " "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")
    endif()
endif()

if(ANDROID_NATIVE_API_LEVEL EQUAL 23)
    set(LUG_STATIC_LIBRARIES true)
endif()


# find Lugdunum and link with it
find_package(LUG REQUIRED system )
if(NOT LUG_FOUND)
    message(FATAL_ERROR "Can't find Lugdunum libraries")
endif()

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
