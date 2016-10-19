cmake_minimum_required(VERSION 3.6.0)

# project name
project(base)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules")

if(ANDROID_NATIVE_API_LEVEL)

    if(ANDROID_NATIVE_API_LEVEL EQUAL 23)
        set(LUG_STATIC_LIBRARIES true)
    endif()

    # find Lugdunum and link with it
    find_package(LUG REQUIRED core system)

    include_directories(${LUG_INCLUDE_DIR})


    add_library(# name defined in AndroidManifest.xml
                sample
                SHARED
                main.cpp
    )

    target_link_libraries(sample
                          log
                          android
                          ${LUG_LIBRARIES}
    )
else()
    # determine the build type
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
    endif()

    # define the executable
    set(EXECUTABLE_NAME "base")

    add_executable(${EXECUTABLE_NAME} main.cpp)

    # find Lugdunum and link with it
    find_package(LUG REQUIRED system)
    if(LUG_FOUND)
        include_directories(${LUG_INCLUDE_DIR})
        target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
    endif()
endif()
