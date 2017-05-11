set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Graphics)

# all source files
set(SRC
    ${SRCROOT}/Graphics.cpp

    ${SRCROOT}/Light/Directional.cpp
    ${SRCROOT}/Light/Light.cpp
    ${SRCROOT}/Light/Point.cpp
    ${SRCROOT}/Light/Spot.cpp

    ${SRCROOT}/Module.cpp
    ${SRCROOT}/Node.cpp

    ${SRCROOT}/Render/Camera.cpp
    ${SRCROOT}/Render/Mesh.cpp
    ${SRCROOT}/Render/Model.cpp
    ${SRCROOT}/Render/Queue.cpp
    ${SRCROOT}/Render/View.cpp

    ${SRCROOT}/Renderer.cpp

    ${SRCROOT}/Scene/MeshInstance.cpp
    ${SRCROOT}/Scene/ModelInstance.cpp
    ${SRCROOT}/Scene/ModelLoader.cpp
    ${SRCROOT}/Scene/MovableCamera.cpp
    ${SRCROOT}/Scene/MovableObject.cpp
    ${SRCROOT}/Scene/Node.cpp
    ${SRCROOT}/Scene/Scene.cpp

    ${SRCROOT}/Vulkan/API/Builder/Device.cpp
    ${SRCROOT}/Vulkan/API/Builder/Instance.cpp

    ${SRCROOT}/Vulkan/API/Buffer.cpp
    ${SRCROOT}/Vulkan/API/CommandBuffer.cpp
    ${SRCROOT}/Vulkan/API/CommandPool.cpp
    ${SRCROOT}/Vulkan/API/DescriptorPool.cpp
    ${SRCROOT}/Vulkan/API/DescriptorSet.cpp
    ${SRCROOT}/Vulkan/API/DescriptorSetLayout.cpp
    ${SRCROOT}/Vulkan/API/Device.cpp
    ${SRCROOT}/Vulkan/API/DeviceMemory.cpp
    ${SRCROOT}/Vulkan/API/Fence.cpp
    ${SRCROOT}/Vulkan/API/Framebuffer.cpp
    ${SRCROOT}/Vulkan/API/Image.cpp
    ${SRCROOT}/Vulkan/API/ImageView.cpp
    ${SRCROOT}/Vulkan/API/Instance.cpp
    ${SRCROOT}/Vulkan/API/Loader.cpp
    ${SRCROOT}/Vulkan/API/Pipeline.cpp
    ${SRCROOT}/Vulkan/API/PipelineLayout.cpp
    ${SRCROOT}/Vulkan/API/Queue.cpp
    ${SRCROOT}/Vulkan/API/RTTI/Enum.cpp
    ${SRCROOT}/Vulkan/API/RenderPass.cpp
    ${SRCROOT}/Vulkan/API/Semaphore.cpp
    ${SRCROOT}/Vulkan/API/ShaderModule.cpp
    ${SRCROOT}/Vulkan/API/Swapchain.cpp

    ${SRCROOT}/Vulkan/Render/BufferPool.cpp
    ${SRCROOT}/Vulkan/Render/Camera.cpp
    ${SRCROOT}/Vulkan/Render/Mesh.cpp
    ${SRCROOT}/Vulkan/Render/Model.cpp
    ${SRCROOT}/Vulkan/Render/Technique/Forward.cpp
    ${SRCROOT}/Vulkan/Render/Technique/Technique.cpp
    ${SRCROOT}/Vulkan/Render/View.cpp
    ${SRCROOT}/Vulkan/Render/Window.cpp

    ${SRCROOT}/Vulkan/Renderer.cpp

    ${SRCROOT}/Vulkan/Requirements/Core.hpp
    ${SRCROOT}/Vulkan/Requirements/Requirements.hpp

    ${SRCROOT}/Vulkan/Vulkan.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Export.hpp
    ${INCROOT}/Graphics.hpp
    ${INCROOT}/Graphics.inl

    ${INCROOT}/Light/Directional.hpp
    ${INCROOT}/Light/Light.hpp
    ${INCROOT}/Light/Light.inl
    ${INCROOT}/Light/Point.hpp
    ${INCROOT}/Light/Spot.hpp

    ${INCROOT}/Module.hpp
    ${INCROOT}/Module.inl
    ${INCROOT}/Node.hpp
    ${INCROOT}/Node.inl

    ${INCROOT}/Render/Camera.hpp
    ${INCROOT}/Render/Camera.inl
    ${INCROOT}/Render/Mesh.hpp
    ${INCROOT}/Render/Mesh.inl
    ${INCROOT}/Render/Model.hpp
    ${INCROOT}/Render/Queue.hpp
    ${INCROOT}/Render/Queue.inl
    ${INCROOT}/Render/Target.hpp
    ${INCROOT}/Render/Target.inl
    ${INCROOT}/Render/Technique/Type.hpp
    ${INCROOT}/Render/View.hpp
    ${INCROOT}/Render/View.inl
    ${INCROOT}/Render/Window.hpp

    ${INCROOT}/Renderer.hpp
    ${INCROOT}/Renderer.inl

    ${INCROOT}/Scene/MeshInstance.hpp
    ${INCROOT}/Scene/MeshInstance.inl
    ${INCROOT}/Scene/ModelInstance.hpp
    ${INCROOT}/Scene/ModelInstance.inl
    ${INCROOT}/Scene/ModelLoader.hpp
    ${INCROOT}/Scene/MovableCamera.hpp
    ${INCROOT}/Scene/MovableObject.hpp
    ${INCROOT}/Scene/MovableObject.inl
    ${INCROOT}/Scene/Node.hpp
    ${INCROOT}/Scene/Node.inl
    ${INCROOT}/Scene/Scene.hpp
    ${INCROOT}/Scene/Scene.inl

    ${INCROOT}/Vulkan/API/Builder/Device.hpp
    ${INCROOT}/Vulkan/API/Builder/Device.inl
    ${INCROOT}/Vulkan/API/Builder/Instance.hpp
    ${INCROOT}/Vulkan/API/Builder/Instance.inl

    ${INCROOT}/Vulkan/API/Buffer.hpp
    ${INCROOT}/Vulkan/API/CommandBuffer.hpp
    ${INCROOT}/Vulkan/API/CommandPool.hpp
    ${INCROOT}/Vulkan/API/CommandPool.inl
    ${INCROOT}/Vulkan/API/DescriptorPool.hpp
    ${INCROOT}/Vulkan/API/DescriptorSet.hpp
    ${INCROOT}/Vulkan/API/DescriptorSetLayout.hpp
    ${INCROOT}/Vulkan/API/Device.hpp
    ${INCROOT}/Vulkan/API/Device.inl
    ${INCROOT}/Vulkan/API/DeviceMemory.hpp
    ${INCROOT}/Vulkan/API/Fence.hpp
    ${INCROOT}/Vulkan/API/Framebuffer.hpp
    ${INCROOT}/Vulkan/API/Image.hpp
    ${INCROOT}/Vulkan/API/ImageView.hpp
    ${INCROOT}/Vulkan/API/Instance.hpp
    ${INCROOT}/Vulkan/API/Instance.inl
    ${INCROOT}/Vulkan/API/Loader.hpp
    ${INCROOT}/Vulkan/API/Pipeline.hpp
    ${INCROOT}/Vulkan/API/PipelineLayout.hpp
    ${INCROOT}/Vulkan/API/Queue.hpp
    ${INCROOT}/Vulkan/API/Queue.inl
    ${INCROOT}/Vulkan/API/RTTI/Enum.hpp
    ${INCROOT}/Vulkan/API/RenderPass.hpp
    ${INCROOT}/Vulkan/API/Semaphore.hpp
    ${INCROOT}/Vulkan/API/ShaderModule.hpp
    ${INCROOT}/Vulkan/API/Swapchain.hpp
    ${INCROOT}/Vulkan/API/Swapchain.inl

    ${INCROOT}/Vulkan/Render/BufferPool.hpp
    ${INCROOT}/Vulkan/Render/Camera.hpp
    ${INCROOT}/Vulkan/Render/Mesh.hpp
    ${INCROOT}/Vulkan/Render/Mesh.inl
    ${INCROOT}/Vulkan/Render/Model.hpp
    ${INCROOT}/Vulkan/Render/Model.inl
    ${INCROOT}/Vulkan/Render/Technique/Forward.hpp
    ${INCROOT}/Vulkan/Render/Technique/Technique.hpp
    ${INCROOT}/Vulkan/Render/View.hpp
    ${INCROOT}/Vulkan/Render/View.inl
    ${INCROOT}/Vulkan/Render/Window.hpp
    ${INCROOT}/Vulkan/Render/Window.inl

    ${INCROOT}/Vulkan/Renderer.hpp
    ${INCROOT}/Vulkan/Renderer.inl
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

message(STATUS "Found Vulkan header: ${VULKAN_INCLUDE_DIR}")
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

    message(STATUS "Found Assimp library: ${ASSIMP_LIBRARY}")
    message(STATUS "Found Assimp includes: ${ASSIMP_INCLUDE_DIR}")
endif()

include_directories(${ASSIMP_INCLUDE_DIR})

if(LUG_OS_ANDROID)
    list(APPEND EXT_LIBRARIES z android ${ASSIMP_LIBRARY_ANDROID})
endif()

# define the target
lug_add_library(lug-graphics
                SOURCES ${SRC} ${INC}
                DEPENDS lug-window lug-system lug-math
                EXTERNAL_LIBS ${EXT_LIBRARIES} ${ASSIMP_LIBRARY}
)
