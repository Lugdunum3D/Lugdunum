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
lug_set_option(BUILD_LONG_TESTS FALSE BOOL "TRUE to enable long unit tests, FALSE to disable long unit tests")
lug_set_option(BUILD_DOCUMENTATION FALSE BOOL "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# set the path of thirdparty
lug_set_option(LUG_THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty" STRING "Choose the path for the thirdparty directory")


# download thirdparty dir if not specified
lug_set_option(LUG_THIRDPARTY_URL "https://thirdparty-dl.lugbench.eu" STRING "Choose the server from which to download the thirdparty directory")
lug_set_option(LUG_ACCEPT_DL OFF BOOL "Choose whether to accept or not the download of the thirdparty directory")

if (NOT EXISTS "${LUG_THIRDPARTY_DIR}" AND NOT "${LUG_ACCEPT_DL}")
    message(
        FATAL_ERROR "You don't have the ${LUG_THIRDPARTY_DIR} directory installed, "
        "but we can download it for your architecture.\n"
        "To protect your privacy the download is not automatic.\n"
        "To proceed and accept the download, re-run cmake with -DLUG_ACCEPT_DL=ON\n"
        "---\n"
        "You can also compile it from source: see https://github.com/Lugdunum3D/ThirdParty-Builder\n"
        "---\n"
        "Thanks, the Lugdunum3D Team."
    )
endif()

if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    find_package(Git)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -n 1 --pretty=format:%h -- thirdparty.yml
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE "THIRDPARTY_LATEST_HASH"
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Thirdparty hash: ${THIRDPARTY_LATEST_HASH}")
    else()
        message(FATAL_ERROR "Can't find the latest revision of `thirdparty.yml`")
    endif()

    string(TOLOWER "${CMAKE_SYSTEM_NAME}" THRIDPARTY_PLATFORM)
    message(STATUS "Downloading ${LUG_THIRDPARTY_URL}/${THIRDPARTY_LATEST_HASH}/${THRIDPARTY_PLATFORM}.zip")
    set(DOWNLOAD_TARGET "${CMAKE_SOURCE_DIR}/thirdparty-${THIRDPARTY_LATEST_HASH}.zip")
    file(DOWNLOAD "${LUG_THIRDPARTY_URL}/${THIRDPARTY_LATEST_HASH}/${THRIDPARTY_PLATFORM}.zip" "${DOWNLOAD_TARGET}")
    message(STATUS "Extracting...")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/thirdparty")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xfz "${DOWNLOAD_TARGET}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/thirdparty"
        RESULT_VARIABLE rv
    )
    if (NOT rv EQUAL 0)
        file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/thirdparty")
        message(FATAL_ERROR "error: extract of '${DOWNLOAD_TARGET}' failed")
    endif()
endif()

# find Vulkan
find_package(Vulkan)
if (NOT VULKAN_INCLUDE_DIR)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/vulkan")
        message(FATAL_ERROR "Can't find vulkan in the thirdparty directory")
    endif()

    set(VULKAN_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/vulkan/include)
    message(STATUS "Found Vulkan: ${VULKAN_INCLUDE_DIR}")
endif()

include_directories(${VULKAN_INCLUDE_DIR})

# find fmt
find_package(Fmt)

if (NOT FMT_INCLUDE_DIR)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/fmt")
        message(FATAL_ERROR "Can't find fmt in the thirdparty directory")
    endif()

    set(FMT_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/fmt/include)
    message(STATUS "Found Fmt: ${FMT_INCLUDE_DIR}")
endif()

include_directories(${FMT_INCLUDE_DIR})

# find shaderc
find_package(Shaderc)

if (NOT SHADERC_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/shaderc")
        message(FATAL_ERROR "Can't find shaderc in the thirdparty directory")
    endif()

    set(SHADERC_ROOT "${LUG_THIRDPARTY_DIR}/shaderc")
    find_package(Shaderc REQUIRED)

    message(STATUS "Found shaderc library: ${SHADERC_LIBRARY}")
    message(STATUS "Found shaderc includes: ${SHADERC_INCLUDE_DIR}")
endif()

include_directories(${SHADERC_INCLUDE_DIR})

include_directories(SYSTEM ext/)

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
install(FILES LICENSE.txt DESTINATION ${INSTALL_MISC_DIR})
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
