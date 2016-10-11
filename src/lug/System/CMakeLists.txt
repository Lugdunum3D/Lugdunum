set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/System)

# all source files
set(SRC
    ${SRCROOT}/Exception.cpp
    ${SRCROOT}/Path.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Debug.hpp
    ${INCROOT}/Exception.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/Path.hpp
)
source_group("inc" FILES ${INC})

if(LUG_OS_LINUX OR LUG_SYSTEM_ANDROID)
	list(APPEND INC ${SRCROOT}/Unix/PathImplUnix.hpp)
	list(APPEND SRC ${SRCROOT}/Unix/PathImplUnix.cpp)
elseif(LUG_OS_WINDOWS)
	list(APPEND INC ${SRCROOT}/Win32/PathImplWin32.hpp)
	list(APPEND SRC ${SRCROOT}/Win32/PathImplWin32.cpp)
else()
	message(FATAL_ERROR "No path implementation for this platform")
endif()

# define the target
lug_add_library(lug-system
                SOURCES ${SRC} ${INC}
)
