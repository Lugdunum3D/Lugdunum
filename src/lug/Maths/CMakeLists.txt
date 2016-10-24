set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Maths)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Maths)

# all source files
 set(SRC

 )
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
    ${INCROOT}/Matrix.hpp
    ${INCROOT}/Matrix.inl
    ${INCROOT}/Vector.hpp
    ${INCROOT}/Vector.inl
)
source_group("inc" FILES ${INC})

# define the target
lug_add_library(lug-maths
                SOURCES ${SRC} ${INC}
)
