set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Graphics)

# all source files
set(SRC
    ${SRCROOT}/Camera.cpp
    ${SRCROOT}/Graphics.cpp
    ${SRCROOT}/MeshInstance.cpp
    ${SRCROOT}/Module.cpp
    ${SRCROOT}/MovableCamera.cpp
    ${SRCROOT}/MovableObject.cpp
    ${SRCROOT}/Node.cpp
    ${SRCROOT}/RenderQueue.cpp
    ${SRCROOT}/RenderView.cpp
    ${SRCROOT}/Scene.cpp
    ${SRCROOT}/SceneNode.cpp
    ${SRCROOT}/Vulkan/Buffer.cpp
    ${SRCROOT}/Vulkan/Camera.cpp
    ${SRCROOT}/Vulkan/CommandBuffer.cpp
    ${SRCROOT}/Vulkan/CommandPool.cpp
    ${SRCROOT}/Vulkan/Device.cpp
    ${SRCROOT}/Vulkan/DeviceMemory.cpp
    ${SRCROOT}/Vulkan/Fence.cpp
    ${SRCROOT}/Vulkan/ForwardRenderTechnique.cpp
    ${SRCROOT}/Vulkan/Framebuffer.cpp
    ${SRCROOT}/Vulkan/Image.cpp
    ${SRCROOT}/Vulkan/ImageView.cpp
    ${SRCROOT}/Vulkan/Instance.cpp
    ${SRCROOT}/Vulkan/Loader.cpp
    ${SRCROOT}/Vulkan/Pipeline.cpp
    ${SRCROOT}/Vulkan/PipelineLayout.cpp
    ${SRCROOT}/Vulkan/Queue.cpp
    ${SRCROOT}/Vulkan/Renderer.cpp
    ${SRCROOT}/Vulkan/RenderTechnique.cpp
    ${SRCROOT}/Vulkan/RenderPass.cpp
    ${SRCROOT}/Vulkan/RenderView.cpp
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
    ${INCROOT}/Camera.hpp
    ${INCROOT}/Camera.inl
    ${INCROOT}/Export.hpp
    ${INCROOT}/Graphics.hpp
    ${INCROOT}/Graphics.inl
    ${INCROOT}/Mesh.hpp
    ${INCROOT}/MeshInstance.hpp
    ${INCROOT}/Module.hpp
    ${INCROOT}/Module.inl
    ${INCROOT}/MovableCamera.hpp
    ${INCROOT}/MovableObject.hpp
    ${INCROOT}/MovableObject.inl
    ${INCROOT}/Node.hpp
    ${INCROOT}/Node.inl
    ${INCROOT}/Renderer.hpp
    ${INCROOT}/RenderQueue.hpp
    ${INCROOT}/RenderTarget.hpp
    ${INCROOT}/RenderTechnique.hpp
    ${INCROOT}/RenderView.hpp
    ${INCROOT}/RenderView.inl
    ${INCROOT}/RenderWindow.hpp
    ${INCROOT}/Scene.hpp
    ${INCROOT}/Scene.inl
    ${INCROOT}/SceneNode.hpp
    ${INCROOT}/SceneNode.inl
    ${INCROOT}/Vulkan/Buffer.hpp
    ${INCROOT}/Vulkan/Camera.hpp
    ${INCROOT}/Vulkan/CommandBuffer.hpp
    ${INCROOT}/Vulkan/CommandPool.hpp
    ${INCROOT}/Vulkan/CommandPool.inl
    ${INCROOT}/Vulkan/Device.hpp
    ${INCROOT}/Vulkan/Device.inl
    ${INCROOT}/Vulkan/DeviceMemory.hpp
    ${INCROOT}/Vulkan/Fence.hpp
    ${INCROOT}/Vulkan/ForwardRenderTechnique.hpp
    ${INCROOT}/Vulkan/Framebuffer.hpp
    ${INCROOT}/Vulkan/Image.hpp
    ${INCROOT}/Vulkan/ImageView.hpp
    ${INCROOT}/Vulkan/Instance.hpp
    ${INCROOT}/Vulkan/Instance.inl
    ${INCROOT}/Vulkan/Loader.hpp
    ${INCROOT}/Vulkan/Pipeline.hpp
    ${INCROOT}/Vulkan/PipelineLayout.hpp
    ${INCROOT}/Vulkan/Queue.hpp
    ${INCROOT}/Vulkan/Renderer.hpp
    ${INCROOT}/Vulkan/Renderer.inl
    ${INCROOT}/Vulkan/RenderPass.hpp
    ${INCROOT}/Vulkan/RenderTechnique.hpp
    ${INCROOT}/Vulkan/RenderView.hpp
    ${INCROOT}/Vulkan/RenderWindow.hpp
    ${INCROOT}/Vulkan/RenderWindow.inl
    ${INCROOT}/Vulkan/Semaphore.hpp
    ${INCROOT}/Vulkan/ShaderModule.hpp
    ${INCROOT}/Vulkan/Swapchain.hpp
    ${INCROOT}/Vulkan/Swapchain.inl
    ${INCROOT}/Vulkan/Vulkan.hpp
)
source_group("inc" FILES ${INC})

if (NOT ENABLE_CI)
    find_package(Vulkan)
    include_directories(${VULKAN_INCLUDE_DIR})
endif()

# define the target
lug_add_library(lug-graphics
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-system lug-math
)
