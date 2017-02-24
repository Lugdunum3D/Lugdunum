# Find Assimp
#
# Below are the output variables:
#  - ASSIMP_INCLUDE_DIR
#  - ASSIMP_LIBRARY
#  - ASSIMP_FOUND

find_path(ASSIMP_INCLUDE_DIR
    NAMES assimp/anim.h
    PATHS $ENV{ASSIMP_ROOT}/include ${ASSIMP_ROOT}/include
    CMAKE_FIND_ROOT_PATH_BOTH
)

find_library(ASSIMP_LIBRARY_RELEASE
    NAMES assimp-vc140-mt assimp
    PATHS $ENV{ASSIMP_ROOT} ${ASSIMP_ROOT}
    PATH_SUFFIXES lib
    CMAKE_FIND_ROOT_PATH_BOTH
)

find_library(ASSIMP_LIBRARY_DEBUG
    NAMES assimp-vc140-mtd assimp
    PATHS $ENV{ASSIMP_ROOT} ${ASSIMP_ROOT}
    PATH_SUFFIXES lib
    CMAKE_FIND_ROOT_PATH_BOTH
)

if(ANDROID)
    find_library(ASSIMP_LIBRARY_ANDROID
        NAMES android_jniiosystem
        PATHS $ENV{ASSIMP_ROOT} ${ASSIMP_ROOT}
        PATH_SUFFIXES lib
        CMAKE_FIND_ROOT_PATH_BOTH
    )

    find_library(ASSIMP_LIBRARY_ANDROID
        NAMES android_jniiosystem
        PATHS $ENV{ASSIMP_ROOT} ${ASSIMP_ROOT}
        PATH_SUFFIXES lib
        CMAKE_FIND_ROOT_PATH_BOTH
    )

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Assimp DEFAULT_MSG ASSIMP_LIBRARY_RELEASE ASSIMP_INCLUDE_DIR)

if(ASSIMP_FOUND)
    set(ASSIMP_LIBRARY debug ${ASSIMP_LIBRARY_DEBUG}
                       optimized ${ASSIMP_LIBRARY_RELEASE}
    )
endif()

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)
