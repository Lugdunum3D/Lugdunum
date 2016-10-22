set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Window)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Window)

# all source files
set(SRC
    ${SRCROOT}/Window.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Event.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/Window.hpp
    ${INCROOT}/WindowHandle.hpp
)
source_group("inc" FILES ${INC})

if(LUG_OS_LINUX)
    list(APPEND INC ${SRCROOT}/Unix/WindowImplX11.hpp)
    list(APPEND SRC ${SRCROOT}/Unix/WindowImplX11.cpp)

    find_package(X11 REQUIRED)
    set(LUG_WINDOW_LIBRARIES ${X11_LIBRARIES})

elseif(LUG_OS_WINDOWS)
    list(APPEND INC ${SRCROOT}/Win32/WindowImplWin32.hpp)
    list(APPEND SRC ${SRCROOT}/Win32/WindowImplWin32.cpp)
else()
    message(FATAL_ERROR "No window implementation for this platform")
endif()

# define the target
lug_add_library(lug-window
                SOURCES ${SRC} ${INC}
                DEPENDS lug-system
                EXTERNAL_LIBS ${X11_LIBRARIES}
)
