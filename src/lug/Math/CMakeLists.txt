set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Math)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Math)

# all source files
set(SRC
    ${SRCROOT}/Matrix.cpp
    ${SRCROOT}/Vector.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Constant.hpp
    ${INCROOT}/Constant.inl
    ${INCROOT}/Export.hpp
    ${INCROOT}/Geometry/Transform.hpp
    ${INCROOT}/Geometry/Transform.inl
    ${INCROOT}/Geometry/Trigonometry.hpp
    ${INCROOT}/Geometry/Trigonometry.inl
    ${INCROOT}/Matrix.hpp
    ${INCROOT}/Matrix.inl
    ${INCROOT}/Quaternion.hpp
    ${INCROOT}/Quaternion.inl
    ${INCROOT}/Vector.hpp
    ${INCROOT}/Vector.inl
)
source_group("inc" FILES ${INC})

# define the target
lug_add_library(lug-math
                SOURCES ${SRC} ${INC}
)
