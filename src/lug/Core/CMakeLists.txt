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

# define the target
lug_add_library(lug-core
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-graphic lug-system
)
