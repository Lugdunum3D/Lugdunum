set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Graphics)

# all source files
set(SRC
    ${SRCROOT}/Camera.cpp
    ${SRCROOT}/DirectionalLight.cpp
    ${SRCROOT}/Graphics.cpp
    ${SRCROOT}/Light.cpp
    ${SRCROOT}/Mesh.cpp
    ${SRCROOT}/MeshInstance.cpp
    ${SRCROOT}/ModelInstance.cpp
    ${SRCROOT}/Module.cpp
    ${SRCROOT}/Model.cpp
    ${SRCROOT}/ModelLoader.cpp
    ${SRCROOT}/MovableCamera.cpp
    ${SRCROOT}/MovableObject.cpp
    ${SRCROOT}/Node.cpp
    ${SRCROOT}/PointLight.cpp
    ${SRCROOT}/Renderer.cpp
    ${SRCROOT}/RenderQueue.cpp
    ${SRCROOT}/RenderView.cpp
    ${SRCROOT}/Scene.cpp
    ${SRCROOT}/SceneNode.cpp
    ${SRCROOT}/Spotlight.cpp
    ${SRCROOT}/Vulkan/Buffer.cpp
    ${SRCROOT}/Vulkan/BufferPool.cpp
    ${SRCROOT}/Vulkan/Camera.cpp
    ${SRCROOT}/Vulkan/CommandBuffer.cpp
    ${SRCROOT}/Vulkan/CommandPool.cpp
    ${SRCROOT}/Vulkan/DescriptorPool.cpp
    ${SRCROOT}/Vulkan/DescriptorSet.cpp
    ${SRCROOT}/Vulkan/DescriptorSetLayout.cpp
    ${SRCROOT}/Vulkan/Device.cpp
    ${SRCROOT}/Vulkan/DeviceMemory.cpp
    ${SRCROOT}/Vulkan/Fence.cpp
    ${SRCROOT}/Vulkan/ForwardRenderTechnique.cpp
    ${SRCROOT}/Vulkan/Framebuffer.cpp
    ${SRCROOT}/Vulkan/Image.cpp
    ${SRCROOT}/Vulkan/ImageView.cpp
    ${SRCROOT}/Vulkan/Instance.cpp
    ${SRCROOT}/Vulkan/Loader.cpp
    ${SRCROOT}/Vulkan/Mesh.cpp
    ${SRCROOT}/Vulkan/Model.cpp
    ${SRCROOT}/Vulkan/Pipeline.cpp
    ${SRCROOT}/Vulkan/PipelineLayout.cpp
    ${SRCROOT}/Vulkan/Queue.cpp
    ${SRCROOT}/Vulkan/Renderer.cpp
    ${SRCROOT}/Vulkan/RenderPass.cpp
    ${SRCROOT}/Vulkan/RenderTechnique.cpp
    ${SRCROOT}/Vulkan/RenderView.cpp
    ${SRCROOT}/Vulkan/RenderWindow.cpp
    ${SRCROOT}/Vulkan/Requirements/Core.hpp
    ${SRCROOT}/Vulkan/Requirements/Requirements.hpp
    ${SRCROOT}/Vulkan/RTTI/Enum.cpp
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
    ${INCROOT}/DirectionalLight.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/Graphics.hpp
    ${INCROOT}/Graphics.inl
    ${INCROOT}/Light.hpp
    ${INCROOT}/Light.inl
    ${INCROOT}/Mesh.hpp
    ${INCROOT}/Mesh.inl
    ${INCROOT}/MeshInstance.hpp
    ${INCROOT}/MeshInstance.inl
    ${INCROOT}/Model.hpp
    ${INCROOT}/ModelInstance.hpp
    ${INCROOT}/ModelInstance.inl
    ${INCROOT}/ModelLoader.hpp
    ${INCROOT}/Module.hpp
    ${INCROOT}/Module.inl
    ${INCROOT}/MovableCamera.hpp
    ${INCROOT}/MovableObject.hpp
    ${INCROOT}/MovableObject.inl
    ${INCROOT}/Node.hpp
    ${INCROOT}/Node.inl
    ${INCROOT}/PointLight.hpp
    ${INCROOT}/Renderer.hpp
    ${INCROOT}/Renderer.inl
    ${INCROOT}/RenderQueue.hpp
    ${INCROOT}/RenderQueue.inl
    ${INCROOT}/RenderTarget.hpp
    ${INCROOT}/RenderTarget.inl
    ${INCROOT}/RenderTechnique.hpp
    ${INCROOT}/RenderView.hpp
    ${INCROOT}/RenderView.inl
    ${INCROOT}/RenderWindow.hpp
    ${INCROOT}/Scene.hpp
    ${INCROOT}/Scene.inl
    ${INCROOT}/SceneNode.hpp
    ${INCROOT}/SceneNode.inl
    ${INCROOT}/Spotlight.hpp
    ${INCROOT}/Vulkan/Buffer.hpp
    ${INCROOT}/Vulkan/BufferPool.hpp
    ${INCROOT}/Vulkan/Camera.hpp
    ${INCROOT}/Vulkan/CommandBuffer.hpp
    ${INCROOT}/Vulkan/CommandPool.hpp
    ${INCROOT}/Vulkan/CommandPool.inl
    ${INCROOT}/Vulkan/DescriptorPool.hpp
    ${INCROOT}/Vulkan/DescriptorSet.hpp
    ${INCROOT}/Vulkan/DescriptorSetLayout.hpp
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
    ${INCROOT}/Vulkan/Mesh.hpp
    ${INCROOT}/Vulkan/Mesh.inl
    ${INCROOT}/Vulkan/Pipeline.hpp
    ${INCROOT}/Vulkan/PipelineLayout.hpp
    ${INCROOT}/Vulkan/Queue.hpp
    ${INCROOT}/Vulkan/Renderer.hpp
    ${INCROOT}/Vulkan/Renderer.inl
    ${INCROOT}/Vulkan/RenderPass.hpp
    ${INCROOT}/Vulkan/RenderTechnique.hpp
    ${INCROOT}/Vulkan/RenderView.hpp
    ${INCROOT}/Vulkan/RenderView.inl
    ${INCROOT}/Vulkan/RenderWindow.hpp
    ${INCROOT}/Vulkan/RenderWindow.inl
    ${INCROOT}/Vulkan/RTTI/Enum.hpp
    ${INCROOT}/Vulkan/Semaphore.hpp
    ${INCROOT}/Vulkan/ShaderModule.hpp
    ${INCROOT}/Vulkan/Swapchain.hpp
    ${INCROOT}/Vulkan/Swapchain.inl
    ${INCROOT}/Vulkan/Vulkan.hpp
)
source_group("inc" FILES ${INC})

find_package(Vulkan)
if (NOT VULKAN_INCLUDE_DIR)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/vulkan")
        message(FATAL_ERROR "Can't find vulkan, call `git submodule update --recursive`")
    endif()

    set(VULKAN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/vulkan/include)
endif()

include_directories(${VULKAN_INCLUDE_DIR})

# Find assimp
if(NOT LUG_OS_ANDROID)
    find_package(Assimp)
endif()

if (NOT ASSIMP_FOUND)
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/thirdparty/assimp")
        message(FATAL_ERROR "Can't find assimp, call `git submodule update --recursive`")
    endif()

    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/assimp/include")

    if (LUG_OS_WINDOWS)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/windows/${ARCH_DIR}")
        find_package(Assimp REQUIRED)
    elseif (LUG_OS_LINUX)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/linux")
        find_package(Assimp REQUIRED)
    elseif (LUG_OS_ANDROID)
        set(ASSIMP_ROOT "${CMAKE_SOURCE_DIR}/thirdparty/assimp/lib/android")
        set(ASSIMP_LIBRARY "${ASSIMP_ROOT}/libassimp.so")

        # Install the libassimp.so alongside lug-graphics.so
        install(FILES ${ASSIMP_LIBRARY}
            DESTINATION lib${LIB_SUFFIX}
        )

        set(ASSIMP_LIBRARY_ANDROID "${ASSIMP_ROOT}/libandroid_jniiosystem.a")
    endif()

endif()

include_directories(${ASSIMP_INCLUDE_DIR})

if(LUG_OS_ANDROID)
    include_directories(${CMAKE_SYSROOT}/usr/include)
    list(APPEND EXT_LIBRARIES z android ${ASSIMP_LIBRARY_ANDROID})
endif()

# define the target
lug_add_library(lug-graphics
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-system lug-math
                EXTERNAL_LIBS ${EXT_LIBRARIES} ${ASSIMP_LIBRARY}
)
