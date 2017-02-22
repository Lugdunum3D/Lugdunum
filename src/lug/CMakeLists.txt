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

    list(APPEND CMAKE_CXX_FLAGS "/WX")
elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
    list(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

    if(LUG_OS_ANDROID)
        list(APPEND CMAKE_CXX_FLAGS "-std=c++14")
    endif()
endif()

string(REPLACE ";" " " "CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS}")

if(CI_BUILD)
    include_directories(${PROJECT_SOURCE_DIR}/test/ci/include)
endif()

# Add parallel build for MSVC
if(LUG_COMPILER_MSVC)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# add the librairies subdirectories
add_subdirectory(Core)
add_subdirectory(Graphics)
add_subdirectory(System)
add_subdirectory(Window)
add_subdirectory(Math)

if(LUG_OS_ANDROID)
    add_subdirectory(Main)
endif()
