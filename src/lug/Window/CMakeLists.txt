set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Window)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Window)

# all source files
set(SRC
    ${SRCROOT}/Window.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Event.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/Window.hpp
    ${INCROOT}/WindowHandle.hpp
)
source_group("inc" FILES ${INC})

set(EXT_LIBRARIES)
if(LUG_OS_LINUX)
    find_package(X11 REQUIRED)
    list(APPEND EXT_LIBRARIES ${X11_LIBRARIES})
    list(APPEND INC ${SRCROOT}/Unix/WindowImplX11.hpp)
    list(APPEND SRC ${SRCROOT}/Unix/WindowImplX11.cpp)
elseif(LUG_OS_WINDOWS)
    list(APPEND INC ${SRCROOT}/Win32/WindowImplWin32.hpp)
    list(APPEND SRC ${SRCROOT}/Win32/WindowImplWin32.cpp)
elseif(LUG_OS_ANDROID)
#    include_directories(${CMAKE_SYSROOT}/usr/include)
    include_directories(/Users/sumenia/Library/Android/sdk/ndk-bundle/platforms/android-24/arch-arm64/usr/include)
    list(APPEND EXT_LIBRARIES android)
    list(APPEND INC ${SRCROOT}/Android/WindowImplAndroid.hpp)
    list(APPEND SRC ${SRCROOT}/Android/WindowImplAndroid.cpp)
else()
    message(FATAL_ERROR "No window implementation for this platform")
endif()

# define the target
lug_add_library(lug-window
                SOURCES ${SRC} ${INC}
                DEPENDS lug-system
                EXTERNAL_LIBS ${EXT_LIBRARIES}
)
