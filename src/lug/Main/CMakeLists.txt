set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Main)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Main)

# all source files
set(SRC

)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
)
source_group("inc" FILES ${INC})

# Include Main cpp files depending on user os
if(LUG_OS_WINDOWS)
    list(APPEND SRC ${SRCROOT}/MainWindows.cpp)
elseif(LUG_OS_ANDROID)
    list(APPEND SRC ${SRCROOT}/MainAndroid.cpp)
endif()

# define the target
lug_add_library(lug-main
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-graphics lug-system
                EXTERNAL_LIBS ""
                FORCE_STATIC TRUE
)
