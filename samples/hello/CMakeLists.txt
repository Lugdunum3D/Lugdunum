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
project(hello)

# use config
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Config.cmake)

# use sample' macros
include(${PROJECT_SOURCE_DIR}/../Macros.cmake)

set(SRC
    src/Application.cpp
    src/main.cpp
)
source_group("src" FILES ${SRC})

set(INC
    include/Application.hpp
)
source_group("inc" FILES ${INC})

set(LUG_RESOURCES
    models/Box0.bin
    models/Box.gltf
    shaders/forward/shader.frag
    shaders/forward/shader.vert
)

include_directories(include)

lug_add_sample(hello
               SOURCES ${SRC} ${INC}
               DEPENDS core graphics system window math
               SHADERS ${SHADERS}
               LUG_RESOURCES ${LUG_RESOURCES}
)

