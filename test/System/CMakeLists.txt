set(SRC_ROOT ${PROJECT_SOURCE_DIR}/System)

set(SRC
    ${SRC_ROOT}/Exception.cpp
    ${SRC_ROOT}/Memory/Memory.cpp
)
source_group("src" FILES ${SRC})

lug_add_test(System
             SOURCES ${SRC}
             DEPENDS lug-system
)
