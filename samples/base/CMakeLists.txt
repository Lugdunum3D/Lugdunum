cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules")

# use macros
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Macros.cmake)

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

if(ANDROID)
    populate_android_infos()
endif()

# project name
project(base)

# use config
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Config.cmake)

# use sample' macros
include(${CMAKE_CURRENT_SOURCE_DIR}/../Macros.cmake)

lug_add_sample(base
               SOURCES main.cpp
               DEPENDS system window
)
