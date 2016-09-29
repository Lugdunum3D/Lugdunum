cmake_minimum_required(VERSION 3.1)

# define a macro that helps defining an option
macro(lug_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# project name
project(Lugdunum CXX)

# version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

add_definitions(-DLUG_VERSION_MAJOR=${VERSION_MAJOR})
add_definitions(-DLUG_VERSION_MINOR=${VERSION_MINOR})
add_definitions(-DLUG_VERSION_PATCH=${VERSION_PATCH})

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# add the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

lug_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build Lugdunum as shared libraries, FALSE to build it as static libraries")

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DLUG_STATIC)
endif()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add the subdirectories
add_subdirectory(src/lug/)

# TODO: setup the install for include files
# TODO: setup the install for misc files (license, readme, FindLUG.cmake)
