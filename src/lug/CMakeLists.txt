# include macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

# define the path of CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules/")

# set the output directory for libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add the librairies subdirectories
add_subdirectory(Core)
add_subdirectory(Graphics)
add_subdirectory(System)
add_subdirectory(Window)
add_subdirectory(Math)

if(LUG_OS_ANDROID)
    add_subdirectory(Main)
endif()
