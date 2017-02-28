cmake_minimum_required(VERSION 3.1)

# include the macros files
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Macros.cmake)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

if(ANDROID)
    populate_android_infos()
endif()

# project name
project(Lugdunum C CXX)

# version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# use cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# include the configuration files
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

if(LUG_OS_ANDROID)
    set(CMAKE_INSTALL_PREFIX "${ANDROID_NDK}/sources/lugdunum")
endif()

# add the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

lug_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build Lugdunum as shared libraries, FALSE to build it as static libraries")
lug_set_option(BUILD_TESTS FALSE BOOL "TRUE to enable unit tests, FALSE to disable unit tests")
lug_set_option(BUILD_DOCUMENTATION FALSE BOOL "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# find fmt
find_package(Fmt)

if (NOT FMT_INCLUDE_DIR)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/fmt")
        message(FATAL_ERROR "Can't find fmt, call `git submodule update --recursive`")
    endif()

    set(FMT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/fmt/include)
endif()

include_directories(${FMT_INCLUDE_DIR})

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

# setup the install of resources (only shaders for the moment)
install(DIRECTORY resources
        DESTINATION ${INSTALL_MISC_DIR}
)

# unit test
if(BUILD_TESTS)
    # Note: enable_testing() MUST be on the top level CMakeLists.txt
    enable_testing()
    add_subdirectory(test/)
endif()

# documentation
if(BUILD_DOCUMENTATION)

    # find doxygen
    find_package(Doxygen)

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

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${INSTALL_MISC_DIR}/doc)
endif()

# TODO: setup the install for license
