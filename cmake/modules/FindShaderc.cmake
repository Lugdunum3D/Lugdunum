# Find shaderc
#
# Below are the output variables:
#  - SHADERC_INCLUDE_DIR
#  - SHADERC_LIBRARY
#  - SHADERC_FOUND


find_path(SHADERC_INCLUDE_DIR
    NAMES shaderc/shaderc.hpp
    PATHS $ENV{SHADERC_ROOT} ${SHADERC_ROOT}
    PATH_SUFFIXES include
    CMAKE_FIND_ROOT_PATH_BOTH
)

find_library(SHADERC_LIBRARY
    NAMES shaderc_combined
    PATHS $ENV{SHADERC_ROOT} ${SHADERC_ROOT}
    PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
    CMAKE_FIND_ROOT_PATH_BOTH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(shaderc FOUND_VAR SHADERC_FOUND REQUIRED_VARS SHADERC_INCLUDE_DIR SHADERC_LIBRARY)

mark_as_advanced(SHADERC_INCLUDE_DIR SHADERC_LIBRARY)
