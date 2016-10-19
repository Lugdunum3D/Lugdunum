# include macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

# define the path of CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules/")

# set the output directory for libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add some definitions
add_definitions(-DNOMINMAX)
add_definitions(-D_USE_MATH_DEFINES)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

if(LUG_COMPILER_MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        list(APPEND CMAKE_CXX_FLAGS "/W4")
    endif()
elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
    list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
endif()

# add the librairies subdirectories
add_subdirectory(Core)
add_subdirectory(Graphics)
add_subdirectory(System)
add_subdirectory(Window)

if(LUG_OS_ANDROID OR LUG_OS_WINDOWS)
    # todo: change that
    set(BUILD_SHARED_LIBS OFF)
    add_subdirectory(Main)
    set(BUILD_SHARED_LIBS ON)
endif()
