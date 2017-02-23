set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Main)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Main)

# all source files
set(SRC

)
source_group("src" FILES ${SRC})

# all header files
set(INC

)
source_group("inc" FILES ${INC})

# Include Main cpp files depending on user os
if(LUG_OS_ANDROID)
#    include_directories(${CMAKE_SYSROOT}/usr/include)
    include_directories(/Users/sumenia/Library/Android/sdk/ndk-bundle/platforms/android-24/arch-arm64/usr/include)
    list(APPEND SRC ${SRCROOT}/MainAndroid.cpp)
    list(APPEND INC ${SRCROOT}/MainAndroid.hpp)
endif()

# define the target
add_library(lug-main STATIC ${SRC} ${INC})

set_target_properties(lug-main PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(lug-main PROPERTIES DEBUG_POSTFIX -d)

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(lug-main PROPERTIES FOLDER "lug")

# setup the install of the library
install(TARGETS lug-main
        ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel)
