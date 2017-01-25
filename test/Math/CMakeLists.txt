set(SRC_ROOT ${PROJECT_SOURCE_DIR}/Math)

set(SRC
    ${SRC_ROOT}/Geometry/Transform.cpp
    ${SRC_ROOT}/Matrix2x2.cpp
    ${SRC_ROOT}/Matrix3x3.cpp
    ${SRC_ROOT}/Matrix4x4.cpp
    ${SRC_ROOT}/Quaternion.cpp
)
source_group("src" FILES ${SRC})

lug_add_test(Math
             SOURCES ${SRC}
             DEPENDS lug-math
)
