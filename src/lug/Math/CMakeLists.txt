set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Math)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Math)

# all source files
 set(SRC
	${SRCROOT}/Quaternion.cpp
 )
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
    ${INCROOT}/Matrix.hpp
    ${INCROOT}/Matrix.inl
    ${INCROOT}/Vector.hpp
    ${INCROOT}/Vector.inl
    ${INCROOT}/Quaternion.hpp
)
source_group("inc" FILES ${INC})

# define the target
lug_add_library(lug-math
                SOURCES ${SRC} ${INC}
)
