set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Graphics)

# all source files
set(SRC
    ${SRCROOT}/Graphics.cpp
    ${SRCROOT}/Module.cpp
    ${SRCROOT}/Vulkan/CommandBuffer.cpp
    ${SRCROOT}/Vulkan/CommandPool.cpp
    ${SRCROOT}/Vulkan/Device.cpp
    ${SRCROOT}/Vulkan/Fence.cpp
    ${SRCROOT}/Vulkan/Image.cpp
    ${SRCROOT}/Vulkan/ImageView.cpp
    ${SRCROOT}/Vulkan/Instance.cpp
    ${SRCROOT}/Vulkan/Loader.cpp
    ${SRCROOT}/Vulkan/Pipeline.cpp
    ${SRCROOT}/Vulkan/Queue.cpp
    ${SRCROOT}/Vulkan/Renderer.cpp
    ${SRCROOT}/Vulkan/RenderPass.cpp
    ${SRCROOT}/Vulkan/RenderWindow.cpp
    ${SRCROOT}/Vulkan/Requirements/Core.hpp
    ${SRCROOT}/Vulkan/Requirements/Requirements.hpp
    ${SRCROOT}/Vulkan/Semaphore.cpp
    ${SRCROOT}/Vulkan/ShaderModule.cpp
    ${SRCROOT}/Vulkan/Swapchain.cpp
    ${SRCROOT}/Vulkan/Vulkan.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
    ${INCROOT}/Graphics.hpp
    ${INCROOT}/Graphics.inl
    ${INCROOT}/Module.hpp
    ${INCROOT}/Module.inl
    ${INCROOT}/Renderer.hpp
    ${INCROOT}/RenderWindow.hpp
    ${INCROOT}/Vulkan/CommandBuffer.hpp
    ${INCROOT}/Vulkan/CommandPool.hpp
    ${INCROOT}/Vulkan/CommandPool.inl
    ${INCROOT}/Vulkan/Device.hpp
    ${INCROOT}/Vulkan/Device.inl
    ${INCROOT}/Vulkan/Fence.hpp
    ${INCROOT}/Vulkan/Image.hpp
    ${INCROOT}/Vulkan/ImageView.hpp
    ${INCROOT}/Vulkan/Instance.hpp
    ${INCROOT}/Vulkan/Instance.inl
    ${INCROOT}/Vulkan/Loader.hpp
    ${INCROOT}/Vulkan/Pipeline.hpp
    ${INCROOT}/Vulkan/Queue.hpp
    ${INCROOT}/Vulkan/Renderer.hpp
    ${INCROOT}/Vulkan/Renderer.inl
    ${INCROOT}/Vulkan/RenderPass.hpp
    ${INCROOT}/Vulkan/RenderWindow.hpp
    ${INCROOT}/Vulkan/Semaphore.hpp
    ${INCROOT}/Vulkan/ShaderModule.hpp
    ${INCROOT}/Vulkan/Swapchain.hpp
    ${INCROOT}/Vulkan/Vulkan.hpp
    ${INCROOT}/Vulkan/Vulkan.inl
)
source_group("inc" FILES ${INC})

if (NOT ENABLE_CI)
    find_package(Vulkan)
    include_directories(${VULKAN_INCLUDE_DIR})
endif()

# define the target
lug_add_library(lug-graphics
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-system
)
