# Find Fmt
#
# Below are the output variables:
#  - FMT_INCLUDE_DIR
#  - FMT_FOUND

find_path(FMT_INCLUDE_DIR
    NAMES fmt/ostream.h
    PATHS $ENV{FMT_ROOT}/include ${FMT_ROOT}/include
    CMAKE_FIND_ROOT_PATH_BOTH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fmt DEFAULT_MSG FMT_INCLUDE_DIR)

mark_as_advanced(FMT_INCLUDE_DIR)
