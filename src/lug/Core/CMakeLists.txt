set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Core)

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
    list(APPEND SRC ${SRCROOT}/Main/MainWindows.cpp)
elseif(LUG_OS_ANDROID)
    list(APPEND SRC ${SRCROOT}/Main/MainAndroid.cpp)
endif()

# define the target
lug_add_library(lug-core
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-graphics lug-system
)
