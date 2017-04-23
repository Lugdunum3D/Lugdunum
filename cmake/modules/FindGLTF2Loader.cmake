# Find glTF2-loader
#
# Below are the output variables:
#  - GLTF2_LOADER_INCLUDE_DIR
#  - GLTF2_LOADER_LIBRARY
#  - GLTF2_LOADER_FOUND


find_path(GLTF2_LOADER_INCLUDE_DIR
    NAMES gltf2/glTF2.hpp
    PATHS $ENV{GLTF2_LOADER_ROOT} ${GLTF2_LOADER_ROOT}
    PATH_SUFFIXES include
    CMAKE_FIND_ROOT_PATH_BOTH
)

find_library(GLTF2_LOADER_LIBRARY
    NAMES gltf2-loader
    PATHS $ENV{GLTF2_LOADER_ROOT} ${GLTF2_LOADER_ROOT}
    PATH_SUFFIXES lib64 lib/${ANDROID_ABI}
    CMAKE_FIND_ROOT_PATH_BOTH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glTF2-loader DEFAULT_MSG GLTF2_LOADER_INCLUDE_DIR)
find_package_handle_standard_args(glTF2-loader DEFAULT_MSG GLTF2_LOADER_LIBRARY)

mark_as_advanced(GLTF2_LOADER_INCLUDE_DIR)
mark_as_advanced(GLTF2_LOADER_LIBRARY)
