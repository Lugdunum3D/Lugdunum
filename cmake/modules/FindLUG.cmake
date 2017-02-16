# This script locates the Lugdunum library
# ------------------------------------
#
# Usage
# -----
#
# When you try to locate the Lugdunum libraries, you must specify which modules you want to use (system, window, graphics, core, etc).
# If none is given, the LUG_LIBRARIES variable will be empty and you'll end up linking to nothing.
# example:
#   find_package(LUG COMPONENTS graphics window system) # find the graphics, window and system modules
#
# You can't enforce a specific version yet.
#
# By default, the dynamic libraries of Lugdunum will be found. To find the static ones instead,
# you must set the LUG_STATIC_LIBRARIES variable to TRUE before calling find_package(LUG ...).
#
# In case of static linking, the LUG_STATIC macro will also be defined by this script.
# example:
#   set(LUG_STATIC_LIBRARIES TRUE)
#   find_package(LUG COMPONENTS system)
#
# If Lugdunum is not installed in a standard path, you can use the LUG_ROOT CMake (or environment) variable
# to tell CMake where Lugdunum is.
#
# Output
# ------
#
# This script defines the following variables:
# - For each specified module XXX (system, window, graphics, core, etc):
#   - LUG_XXX_LIBRARY_DEBUG:   the name of the debug library of the xxx module (set to LUG_XXX_LIBRARY_RELEASE is no debug version is found)
#   - LUG_XXX_LIBRARY_RELEASE: the name of the release library of the xxx module (set to LUG_XXX_LIBRARY_DEBUG is no release version is found)
#   - LUG_XXX_LIBRARY:         the name of the library to link to for the xxx module (includes both debug and optimized names if necessary)
#   - LUG_XXX_FOUND:           true if either the debug or release library of the xxx module is found
# - LUG_LIBRARIES:    the list of all libraries corresponding to the required modules
# - LUG_FOUND:        true if all the required modules are found
# - LUG_INCLUDE_DIR:  the path where Lugdunum headers are located (the directory containing the lug/Config.hpp file)
# - LUG_RESOURCES_DIR: the path where Lugdunum resources are located
#
# example:
#   find_package(LUG COMPONENTS system window graphics REQUIRED)
#   include_directories(${LUG_INCLUDE_DIR})
#   add_executable(myapp ...)
#   target_link_libraries(myapp ${LUG_LIBRARIES})

# define the LUG_STATIC macro if static build was chosen
if(LUG_STATIC_LIBRARIES)
    add_definitions(-DLUG_STATIC)
endif()

# define the list of search paths for headers and libraries
set(FIND_LUG_PATHS
    ${LUG_ROOT}
    $ENV{LUG_ROOT}
    $ENV{ANDROID_NDK}/sources/lugdunum
    ${ANDROID_NDK}/sources/lugdunum
    /usr/local
    /usr
    /opt/local
    /opt
)

# define the list of search paths for miscs
set(FIND_LUG_MICS_PATHS
    ${LUG_ROOT}
    $ENV{LUG_ROOT}
    $ENV{ANDROID_NDK}/sources/lugdunum
    ${ANDROID_NDK}/sources/lugdunum
    /usr/local/share/lug
)

# find the LUG include directory
find_path(LUG_INCLUDE_DIR lug/Config.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_LUG_PATHS}
          CMAKE_FIND_ROOT_PATH_BOTH
)

# find the LUG resources directory
find_path(LUG_RESOURCES_DIR shaders
          PATH_SUFFIXES resources
          PATHS ${FIND_LUG_MICS_PATHS}
          CMAKE_FIND_ROOT_PATH_BOTH
)

set(LUG_FOUND TRUE) # will be set to false if one of the required modules is not found

if(LUG_OS_ANDROID)
    # this will append `lug-main` to the components to find if we are on WINDOWS or ANDROID
    # lug-main provides a wrapper for the main functions of Android and Windows to provide
    # an uniform int main(int ac, char *[]av) across platforms
    list(APPEND LUG_FIND_COMPONENTS "main")
endif()

# find the requested modules
foreach(FIND_LUG_COMPONENT ${LUG_FIND_COMPONENTS})
    string(TOLOWER ${FIND_LUG_COMPONENT} FIND_LUG_COMPONENT_LOWER)
    string(TOUPPER ${FIND_LUG_COMPONENT} FIND_LUG_COMPONENT_UPPER)
    set(FIND_LUG_COMPONENT_NAME lug-${FIND_LUG_COMPONENT_LOWER})

    if(FIND_LUG_COMPONENT_LOWER STREQUAL "main")
        # release library
        find_library(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE
                NAMES ${FIND_LUG_COMPONENT_NAME}
                PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
                PATHS ${FIND_LUG_PATHS}
                CMAKE_FIND_ROOT_PATH_BOTH
        )

        # debug library
        find_library(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG
                NAMES ${FIND_LUG_COMPONENT_NAME}-d
                PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
                PATHS ${FIND_LUG_PATHS}
                CMAKE_FIND_ROOT_PATH_BOTH
        )
    else()
        # static release library
        find_library(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE
                     NAMES ${FIND_LUG_COMPONENT_NAME}-s
                     PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
                     PATHS ${FIND_LUG_PATHS}
                     CMAKE_FIND_ROOT_PATH_BOTH
        )

        # static debug library
        find_library(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG
                     NAMES ${FIND_LUG_COMPONENT_NAME}-s-d
                     PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
                     PATHS ${FIND_LUG_PATHS}
                     CMAKE_FIND_ROOT_PATH_BOTH
        )

        # dynamic release library
        find_library(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
                     NAMES ${FIND_LUG_COMPONENT_NAME}
                     PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
                     PATHS ${FIND_LUG_PATHS}
                     CMAKE_FIND_ROOT_PATH_BOTH
        )

        # dynamic debug library
        find_library(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG
                     NAMES ${FIND_LUG_COMPONENT_NAME}-d
                     PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
                     PATHS ${FIND_LUG_PATHS}
                     CMAKE_FIND_ROOT_PATH_BOTH
        )
    endif()

    # choose the entries that fit the requested link type
    if(LUG_STATIC_LIBRARIES)
        if(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE)
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE})
        endif()

        if(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG)
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG})
        endif()
    else()
        if(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE)
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE})
        endif()

        if(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG)
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG})
        endif()
    endif()

    if (LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG OR LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE)
        # library found
        set(LUG_${FIND_LUG_COMPONENT_UPPER}_FOUND TRUE)

        # if both are found, set LUG_XXX_LIBRARY to contain both
        if (LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG AND LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE)
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY debug ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG}
                                                        optimized ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE})
        endif()

        # if only one debug/release variant is found, set the other to be equal to the found one
        if (LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG AND NOT LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE)
            # debug and not release
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG})
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY         ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG})
        endif()

        if (LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE AND NOT LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG)
            # release and not debug
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE})
            set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY       ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE})
        endif()
    else()
        # library not found
        set(LUG_FOUND FALSE)
        set(LUG_${FIND_LUG_COMPONENT_UPPER}_FOUND FALSE)
        set(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY "")
        set(FIND_LUG_MISSING "${FIND_LUG_MISSING} LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY")
    endif()

    # mark as advanced
    MARK_AS_ADVANCED(LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY
                     LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_RELEASE
                     LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DEBUG
                     LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_RELEASE
                     LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_STATIC_DEBUG
                     LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_RELEASE
                     LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY_DYNAMIC_DEBUG
    )

    # add to the global list of libraries
    if(FIND_LUG_COMPONENT_LOWER STREQUAL "main")
        list(APPEND LUG_LIBRARIES "-Wl,--whole-archive ${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY} -Wl,--no-whole-archive")
    else()
        list(APPEND LUG_LIBRARIES "${LUG_${FIND_LUG_COMPONENT_UPPER}_LIBRARY}")
    endif()

endforeach()

if (LUG_FOUND)
    message(STATUS "Found Lugdunum headers in ${LUG_INCLUDE_DIR}")
    message(STATUS "Found Lugdunum libraries in ${LUG_LIBRARIES}")
    message(STATUS "Found Lugdunum resources in ${LUG_RESOURCES_DIR}")
else()
    # include directory or library not found
    set(FIND_LUG_ERROR "Could NOT find Lugdunum (missing: ${FIND_LUG_MISSING})")

    if(LUG_FIND_REQUIRED)
        message(FATAL_ERROR ${FIND_LUG_ERROR})
    else()
        message("${FIND_LUG_ERROR}")
    endif()
endif()
