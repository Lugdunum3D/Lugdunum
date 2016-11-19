set(SRC_ROOT ${PROJECT_SOURCE_DIR}/Math)

set(SRC
    ${SRC_ROOT}/Matrix.cpp
)
source_group("src" FILES ${SRC})

lug_add_test(Math
             SOURCES ${SRC}
             DEPENDS lug-math
)
