cmake_minimum_required(VERSION 3.1)

# define a macro that helps defining an option
macro(lug_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

if(ANDROID)
    set(ANDROID_ABI arm64-v8a)
    message(STATUS "ANDROID_ABI : ${ANDROID_ABI}")

    set(ANDROID_TOOLCHAIN clang)
    message(STATUS "ANDROID_TOOLCHAIN : ${ANDROID_TOOLCHAIN}")

    lug_set_option(ANDROID_PLATFORM android-24 STRING "Target specific android version target (default : android-24)")
    if(NOT ANDROID_PLATFORM MATCHES android-2[3-9])
        message(FATAL_ERROR "Invalid android version target : ${ANDROID_PLATFORM}")
    endif()
    message(STATUS "ANDROID_PLATFORM : ${ANDROID_PLATFORM}")

    if(ANDROID_PLATFORM MATCHES android-23)
        message(STATUS "target android-23, forcing static lib")
        set(BUILD_SHARED_LIBS false)
    endif()
    lug_set_option(ANDROID_STL c++_shared STRING "Choose which Android STL to use (default : c++_shared)")
    if(NOT ANDROID_STL STREQUAL c++_static AND NOT ANDROID_STL STREQUAL c++_shared)
        message(FATAL_ERROR "Invalid STL : ${ANDROID_STL}")
    endif()

    if(ANDROID_PLATFORM STREQUAL android-23 AND ANDROID_STL STREQUAL c++_shared)
        message(FATAL_ERROR "android-23 and c++_shared are mutually exclusive")
    endif()
    message(STATUS "ANDROID_STL : ${ANDROID_STL}")

    set(ANDROID_CPP_FEATURES "rtti exceptions")
    message(STATUS "ANDROID_CPP_FEATURES : ${ANDROID_CPP_FEATURES}")

    set(LIB_SUFFIX "/${ANDROID_ABI}")
endif()


# project name
project(Lugdunum C CXX)

# version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

add_definitions(-DLUG_VERSION_MAJOR=${VERSION_MAJOR})
add_definitions(-DLUG_VERSION_MINOR=${VERSION_MINOR})
add_definitions(-DLUG_VERSION_PATCH=${VERSION_PATCH})

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# add the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# find doxygen
find_package(Doxygen)

lug_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build Lugdunum as shared libraries, FALSE to build it as static libraries")
lug_set_option(ENABLE_TEST FALSE BOOL "TRUE to enable unit tests, FALSE to disable unit tests")
lug_set_option(BUILD_DOCUMENTATION FALSE BOOL "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DLUG_STATIC)
endif()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add the subdirectories
add_subdirectory(src/lug/)

# setup the install of headers
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl" PATTERN "*.h" PATTERN "*.cc"
)

# setup the install of misc files
install(FILES README.md DESTINATION ${INSTALL_MISC_DIR})
install(FILES cmake/modules/FindLUG.cmake DESTINATION ${INSTALL_MISC_DIR}/cmake/modules)

# unit test
if(ENABLE_TEST)
    # Note: enable_testing() MUST be on the top level CMakeLists.txt
    enable_testing()

    add_definitions(-DGTEST_CREATE_SHARED_LIBRARY=1)

    set(BUILD_GMOCK ON)
    add_subdirectory(lib/googletest)

    add_subdirectory(test/)
endif()

# documentation
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()

# TODO: setup the install for license
