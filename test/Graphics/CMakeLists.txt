set(SRC_ROOT ${PROJECT_SOURCE_DIR}/Graphics)

set(SRC
    ${SRC_ROOT}/Vulkan/Shaders.cpp
)
source_group("src" FILES ${SRC})

set(SHADERS
    forward/shader.vert
    forward/shader.frag
)

lug_add_test(Graphics
             SOURCES ${SRC}
             SHADERS ${SHADERS}
             DEPENDS lug-system lug-graphics lug-core
)
