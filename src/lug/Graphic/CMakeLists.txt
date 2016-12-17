set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Graphic)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Graphic)

# all source files
set(SRC
    ${SRCROOT}/Graphic.cpp
    ${SRCROOT}/Module.cpp
    ${SRCROOT}/Vulkan/Instance.cpp
    ${SRCROOT}/Vulkan/Loader.cpp
    ${SRCROOT}/Vulkan/Renderer.cpp
    ${SRCROOT}/Vulkan/Vulkan.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
    ${INCROOT}/Graphic.hpp
    ${INCROOT}/Graphic.inl
    ${INCROOT}/Module.hpp
    ${INCROOT}/Renderer.hpp
    ${INCROOT}/Vulkan/Instance.hpp
    ${INCROOT}/Vulkan/Loader.hpp
    ${INCROOT}/Vulkan/Renderer.hpp
    ${INCROOT}/Vulkan/Vulkan.hpp
)
source_group("inc" FILES ${INC})

find_package(Vulkan)
include_directories(${VULKAN_INCLUDE_DIR})

# define the target
lug_add_library(lug-graphic
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-system
)
