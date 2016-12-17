set(SRC_ROOT ${PROJECT_SOURCE_DIR}/System)

set(SRC
    ${SRC_ROOT}/Exception.cpp
    ${SRC_ROOT}/Logger/Formatter.cpp
    ${SRC_ROOT}/Logger/Logger.cpp
    ${SRC_ROOT}/Logger/OstreamHandler.cpp
    ${SRC_ROOT}/Memory/MemoryRawPointer.cpp
)
source_group("src" FILES ${SRC})

lug_add_test(System
             SOURCES ${SRC}
             DEPENDS lug-system
)
