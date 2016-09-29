set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/System)

# all source files
set(SRC

)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
)
source_group("inc" FILES ${INC})

# define the target
lug_add_library(lug-system
                SOURCES ${SRC} ${INC}
)
