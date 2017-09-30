set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/Graphics)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/Graphics)

# all source files
set(SRC
    ${SRCROOT}/Graphics.cpp

    ${SRCROOT}/Loader.cpp
    ${SRCROOT}/Builder/Camera.cpp
    ${SRCROOT}/Builder/Light.cpp
    ${SRCROOT}/Builder/Material.cpp
    ${SRCROOT}/Builder/Mesh.cpp
    ${SRCROOT}/Builder/Scene.cpp
    ${SRCROOT}/Builder/SkyBox.cpp
    ${SRCROOT}/Builder/Texture.cpp

    ${SRCROOT}/Module.cpp
    ${SRCROOT}/Node.cpp
    ${SRCROOT}/GltfLoader.cpp
    ${SRCROOT}/Resource.cpp
    ${SRCROOT}/ResourceManager.cpp

    ${SRCROOT}/Render/Camera/Camera.cpp
    ${SRCROOT}/Render/Camera/Orthographic.cpp
    ${SRCROOT}/Render/Camera/Perspective.cpp

    ${SRCROOT}/Render/Light.cpp
    ${SRCROOT}/Render/Material.cpp
    ${SRCROOT}/Render/Mesh.cpp
    ${SRCROOT}/Render/Queue.cpp
    ${SRCROOT}/Render/SkyBox.cpp
    ${SRCROOT}/Render/Texture.cpp
    ${SRCROOT}/Render/View.cpp

    ${SRCROOT}/Renderer.cpp

    ${SRCROOT}/Scene/Node.cpp
    ${SRCROOT}/Scene/Scene.cpp

    ${SRCROOT}/Vulkan/API/Builder/Buffer.cpp
    ${SRCROOT}/Vulkan/API/Builder/CommandBuffer.cpp
    ${SRCROOT}/Vulkan/API/Builder/CommandPool.cpp
    ${SRCROOT}/Vulkan/API/Builder/DescriptorPool.cpp
    ${SRCROOT}/Vulkan/API/Builder/DescriptorSet.cpp
    ${SRCROOT}/Vulkan/API/Builder/DescriptorSetLayout.cpp
    ${SRCROOT}/Vulkan/API/Builder/Device.cpp
    ${SRCROOT}/Vulkan/API/Builder/DeviceMemory.cpp
    ${SRCROOT}/Vulkan/API/Builder/Fence.cpp
    ${SRCROOT}/Vulkan/API/Builder/Framebuffer.cpp
    ${SRCROOT}/Vulkan/API/Builder/GraphicsPipeline.cpp
    ${SRCROOT}/Vulkan/API/Builder/Image.cpp
    ${SRCROOT}/Vulkan/API/Builder/ImageView.cpp
    ${SRCROOT}/Vulkan/API/Builder/Instance.cpp
    ${SRCROOT}/Vulkan/API/Builder/PipelineLayout.cpp
    ${SRCROOT}/Vulkan/API/Builder/RenderPass.cpp
    ${SRCROOT}/Vulkan/API/Builder/Sampler.cpp
    ${SRCROOT}/Vulkan/API/Builder/Semaphore.cpp
    ${SRCROOT}/Vulkan/API/Builder/ShaderModule.cpp
    ${SRCROOT}/Vulkan/API/Builder/Surface.cpp
    ${SRCROOT}/Vulkan/API/Builder/Swapchain.cpp

    ${SRCROOT}/Vulkan/API/Buffer.cpp
    ${SRCROOT}/Vulkan/API/CommandBuffer/Buffer.cpp
    ${SRCROOT}/Vulkan/API/CommandBuffer/DescriptorSet.cpp
    ${SRCROOT}/Vulkan/API/CommandBuffer/Pipeline.cpp
    ${SRCROOT}/Vulkan/API/CommandBuffer/RenderPass.cpp
    ${SRCROOT}/Vulkan/API/CommandBuffer.cpp
    ${SRCROOT}/Vulkan/API/CommandPool.cpp
    ${SRCROOT}/Vulkan/API/DescriptorPool.cpp
    ${SRCROOT}/Vulkan/API/DescriptorSet.cpp
    ${SRCROOT}/Vulkan/API/DescriptorSetLayout.cpp
    ${SRCROOT}/Vulkan/API/Device.cpp
    ${SRCROOT}/Vulkan/API/DeviceMemory.cpp
    ${SRCROOT}/Vulkan/API/Fence.cpp
    ${SRCROOT}/Vulkan/API/Framebuffer.cpp
    ${SRCROOT}/Vulkan/API/GraphicsPipeline.cpp
    ${SRCROOT}/Vulkan/API/Image.cpp
    ${SRCROOT}/Vulkan/API/ImageView.cpp
    ${SRCROOT}/Vulkan/API/Instance.cpp
    ${SRCROOT}/Vulkan/API/Loader.cpp
    ${SRCROOT}/Vulkan/API/PipelineLayout.cpp
    ${SRCROOT}/Vulkan/API/Queue.cpp
    ${SRCROOT}/Vulkan/API/QueueFamily.cpp
    ${SRCROOT}/Vulkan/API/RTTI/Enum.cpp
    ${SRCROOT}/Vulkan/API/RenderPass.cpp
    ${SRCROOT}/Vulkan/API/Sampler.cpp
    ${SRCROOT}/Vulkan/API/Semaphore.cpp
    ${SRCROOT}/Vulkan/API/ShaderModule.cpp
    ${SRCROOT}/Vulkan/API/Surface.cpp
    ${SRCROOT}/Vulkan/API/Swapchain.cpp

    ${SRCROOT}/Vulkan/Builder/Material.cpp
    ${SRCROOT}/Vulkan/Builder/Mesh.cpp
    ${SRCROOT}/Vulkan/Builder/Texture.cpp
    ${SRCROOT}/Vulkan/Builder/SkyBox.cpp

    ${SRCROOT}/Vulkan/Render/BufferPool/Camera.cpp
    ${SRCROOT}/Vulkan/Render/BufferPool/Light.cpp
    ${SRCROOT}/Vulkan/Render/BufferPool/Material.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/Camera.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/Light.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/Material.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/MaterialTextures.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/SkyBox.cpp
    ${SRCROOT}/Vulkan/Render/DescriptorSetPool/GuiTexture.cpp
    ${SRCROOT}/Vulkan/Render/Material.cpp
    ${SRCROOT}/Vulkan/Render/SkyBox.cpp

    ${SRCROOT}/Vulkan/Gui.cpp

    ${SRCROOT}/Vulkan/Render/Mesh.cpp
    ${SRCROOT}/Vulkan/Render/Pipeline.cpp
    ${SRCROOT}/Vulkan/Render/Pipeline/ShaderBuilder.cpp
    ${SRCROOT}/Vulkan/Render/Queue.cpp
    ${SRCROOT}/Vulkan/Render/Technique/Forward.cpp
    ${SRCROOT}/Vulkan/Render/Technique/Technique.cpp
    ${SRCROOT}/Vulkan/Render/SkyBox.cpp
    ${SRCROOT}/Vulkan/Render/Texture.cpp
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
    ${INCROOT}/Loader.hpp
    ${INCROOT}/GltfLoader.hpp
    ${INCROOT}/Resource.hpp
    ${INCROOT}/Resource.inl
    ${INCROOT}/ResourceManager.hpp
    ${INCROOT}/ResourceManager.inl

    ${INCROOT}/Builder/Camera.hpp
    ${INCROOT}/Builder/Camera.inl
    ${INCROOT}/Builder/Light.hpp
    ${INCROOT}/Builder/Light.inl
    ${INCROOT}/Builder/Material.hpp
    ${INCROOT}/Builder/Material.inl
    ${INCROOT}/Builder/Mesh.hpp
    ${INCROOT}/Builder/Mesh.inl
    ${INCROOT}/Builder/Scene.hpp
    ${INCROOT}/Builder/Scene.inl
    ${INCROOT}/Builder/Texture.hpp
    ${INCROOT}/Builder/Texture.inl
    ${INCROOT}/Builder/SkyBox.hpp
    ${INCROOT}/Builder/SkyBox.inl

    ${INCROOT}/Module.hpp
    ${INCROOT}/Module.inl
    ${INCROOT}/Node.hpp
    ${INCROOT}/Node.inl

    ${INCROOT}/Render/Camera/Camera.hpp
    ${INCROOT}/Render/Camera/Camera.inl
    ${INCROOT}/Render/Camera/Orthographic.hpp
    ${INCROOT}/Render/Camera/Orthographic.inl
    ${INCROOT}/Render/Camera/Perspective.hpp
    ${INCROOT}/Render/Camera/Perspective.inl

    ${INCROOT}/Render/DirtyObject.hpp
    ${INCROOT}/Render/DirtyObject.inl
    ${INCROOT}/Render/Light.hpp
    ${INCROOT}/Render/Light.inl
    ${INCROOT}/Render/Material.hpp
    ${INCROOT}/Render/Material.inl
    ${INCROOT}/Render/Mesh.hpp
    ${INCROOT}/Render/Mesh.inl
    ${INCROOT}/Render/Queue.hpp
    ${INCROOT}/Render/SkyBox.hpp
    ${INCROOT}/Render/SkyBox.inl
    ${INCROOT}/Render/Target.hpp
    ${INCROOT}/Render/Target.inl
    ${INCROOT}/Render/Technique/Type.hpp
    ${INCROOT}/Render/Texture.hpp
    ${INCROOT}/Render/View.hpp
    ${INCROOT}/Render/View.inl
    ${INCROOT}/Render/Window.hpp

    ${INCROOT}/Renderer.hpp
    ${INCROOT}/Renderer.inl

    ${INCROOT}/Scene/Node.hpp
    ${INCROOT}/Scene/Node.inl
    ${INCROOT}/Scene/Scene.hpp
    ${INCROOT}/Scene/Scene.inl

    ${INCROOT}/Vulkan/API/Builder/Buffer.hpp
    ${INCROOT}/Vulkan/API/Builder/Buffer.inl
    ${INCROOT}/Vulkan/API/Builder/CommandBuffer.hpp
    ${INCROOT}/Vulkan/API/Builder/CommandBuffer.inl
    ${INCROOT}/Vulkan/API/Builder/CommandPool.hpp
    ${INCROOT}/Vulkan/API/Builder/CommandPool.inl
    ${INCROOT}/Vulkan/API/Builder/DescriptorPool.hpp
    ${INCROOT}/Vulkan/API/Builder/DescriptorPool.inl
    ${INCROOT}/Vulkan/API/Builder/DescriptorSet.hpp
    ${INCROOT}/Vulkan/API/Builder/DescriptorSet.inl
    ${INCROOT}/Vulkan/API/Builder/DescriptorSetLayout.hpp
    ${INCROOT}/Vulkan/API/Builder/DescriptorSetLayout.inl
    ${INCROOT}/Vulkan/API/Builder/Device.hpp
    ${INCROOT}/Vulkan/API/Builder/Device.inl
    ${INCROOT}/Vulkan/API/Builder/DeviceMemory.hpp
    ${INCROOT}/Vulkan/API/Builder/DeviceMemory.inl
    ${INCROOT}/Vulkan/API/Builder/Fence.hpp
    ${INCROOT}/Vulkan/API/Builder/Fence.inl
    ${INCROOT}/Vulkan/API/Builder/Framebuffer.hpp
    ${INCROOT}/Vulkan/API/Builder/Framebuffer.inl
    ${INCROOT}/Vulkan/API/Builder/GraphicsPipeline.hpp
    ${INCROOT}/Vulkan/API/Builder/GraphicsPipeline.inl
    ${INCROOT}/Vulkan/API/Builder/Image.hpp
    ${INCROOT}/Vulkan/API/Builder/Image.inl
    ${INCROOT}/Vulkan/API/Builder/ImageView.hpp
    ${INCROOT}/Vulkan/API/Builder/ImageView.inl
    ${INCROOT}/Vulkan/API/Builder/Instance.hpp
    ${INCROOT}/Vulkan/API/Builder/Instance.inl
    ${INCROOT}/Vulkan/API/Builder/PipelineLayout.hpp
    ${INCROOT}/Vulkan/API/Builder/PipelineLayout.inl
    ${INCROOT}/Vulkan/API/Builder/RenderPass.hpp
    ${INCROOT}/Vulkan/API/Builder/Sampler.hpp
    ${INCROOT}/Vulkan/API/Builder/Sampler.inl
    ${INCROOT}/Vulkan/API/Builder/Semaphore.hpp
    ${INCROOT}/Vulkan/API/Builder/ShaderModule.hpp
    ${INCROOT}/Vulkan/API/Builder/Surface.hpp
    ${INCROOT}/Vulkan/API/Builder/Surface.inl
    ${INCROOT}/Vulkan/API/Builder/Swapchain.hpp
    ${INCROOT}/Vulkan/API/Builder/Swapchain.inl

    ${INCROOT}/Vulkan/API/Buffer.hpp
    ${INCROOT}/Vulkan/API/Buffer.inl
    ${INCROOT}/Vulkan/API/CommandBuffer/Buffer.inl
    ${INCROOT}/Vulkan/API/CommandBuffer/DescriptorSet.inl
    ${INCROOT}/Vulkan/API/CommandBuffer/Pipeline.inl
    ${INCROOT}/Vulkan/API/CommandBuffer/RenderPass.inl
    ${INCROOT}/Vulkan/API/CommandBuffer.hpp
    ${INCROOT}/Vulkan/API/CommandPool.hpp
    ${INCROOT}/Vulkan/API/CommandPool.inl
    ${INCROOT}/Vulkan/API/DescriptorPool.hpp
    ${INCROOT}/Vulkan/API/DescriptorSet.hpp
    ${INCROOT}/Vulkan/API/DescriptorSetLayout.hpp
    ${INCROOT}/Vulkan/API/DescriptorSetLayout.inl
    ${INCROOT}/Vulkan/API/Device.hpp
    ${INCROOT}/Vulkan/API/Device.inl
    ${INCROOT}/Vulkan/API/DeviceMemory.hpp
    ${INCROOT}/Vulkan/API/DeviceMemory.inl
    ${INCROOT}/Vulkan/API/Fence.hpp
    ${INCROOT}/Vulkan/API/Fence.inl
    ${INCROOT}/Vulkan/API/Framebuffer.hpp
    ${INCROOT}/Vulkan/API/GraphicsPipeline.hpp
    ${INCROOT}/Vulkan/API/Image.hpp
    ${INCROOT}/Vulkan/API/Image.inl
    ${INCROOT}/Vulkan/API/ImageView.hpp
    ${INCROOT}/Vulkan/API/ImageView.inl
    ${INCROOT}/Vulkan/API/Instance.hpp
    ${INCROOT}/Vulkan/API/Instance.inl
    ${INCROOT}/Vulkan/API/Loader.hpp
    ${INCROOT}/Vulkan/API/PipelineLayout.hpp
    ${INCROOT}/Vulkan/API/PipelineLayout.inl
    ${INCROOT}/Vulkan/API/Queue.hpp
    ${INCROOT}/Vulkan/API/QueueFamily.hpp
    ${INCROOT}/Vulkan/API/QueueFamily.inl
    ${INCROOT}/Vulkan/API/RTTI/Enum.hpp
    ${INCROOT}/Vulkan/API/RenderPass.hpp
    ${INCROOT}/Vulkan/API/Sampler.hpp
    ${INCROOT}/Vulkan/API/Semaphore.hpp
    ${INCROOT}/Vulkan/API/ShaderModule.hpp
    ${INCROOT}/Vulkan/API/Surface.hpp
    ${INCROOT}/Vulkan/API/Swapchain.hpp
    ${INCROOT}/Vulkan/API/Swapchain.inl

    ${INCROOT}/Vulkan/Builder/Material.hpp
    ${INCROOT}/Vulkan/Builder/Mesh.hpp
    ${INCROOT}/Vulkan/Builder/SkyBox.hpp
    ${INCROOT}/Vulkan/Builder/Texture.hpp

    ${INCROOT}/Vulkan/Render/BufferPool/BufferPool.hpp
    ${INCROOT}/Vulkan/Render/BufferPool/BufferPool.inl
    ${INCROOT}/Vulkan/Render/BufferPool/Camera.hpp
    ${INCROOT}/Vulkan/Render/BufferPool/Chunk.hpp
    ${INCROOT}/Vulkan/Render/BufferPool/Chunk.inl
    ${INCROOT}/Vulkan/Render/BufferPool/Light.hpp
    ${INCROOT}/Vulkan/Render/BufferPool/Material.hpp
    ${INCROOT}/Vulkan/Render/BufferPool/SubBuffer.hpp
    ${INCROOT}/Vulkan/Render/BufferPool/SubBuffer.inl
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/Camera.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/DescriptorSet.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/DescriptorSet.inl
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.inl
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/Light.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/Material.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/MaterialTextures.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/SkyBox.hpp
    ${INCROOT}/Vulkan/Render/DescriptorSetPool/GuiTexture.hpp
    ${INCROOT}/Vulkan/Render/Material.hpp
    ${INCROOT}/Vulkan/Render/SkyBox.hpp
    ${INCROOT}/Vulkan/Render/SkyBox.inl

    ${INCROOT}/Vulkan/Gui.hpp

    ${INCROOT}/Vulkan/Render/Mesh.hpp
    ${INCROOT}/Vulkan/Render/Mesh.inl
    ${INCROOT}/Vulkan/Render/Pipeline.hpp
    ${INCROOT}/Vulkan/Render/Pipeline.inl
    ${INCROOT}/Vulkan/Render/Queue.hpp
    ${INCROOT}/Vulkan/Render/Technique/Forward.hpp
    ${INCROOT}/Vulkan/Render/Technique/Technique.hpp
    ${INCROOT}/Vulkan/Render/Texture.hpp
    ${INCROOT}/Vulkan/Render/Texture.inl
    ${INCROOT}/Vulkan/Render/View.hpp
    ${INCROOT}/Vulkan/Render/View.inl
    ${INCROOT}/Vulkan/Render/Window.hpp
    ${INCROOT}/Vulkan/Render/Window.inl

    ${INCROOT}/Vulkan/Renderer.hpp
    ${INCROOT}/Vulkan/Renderer.inl
    ${INCROOT}/Vulkan/Vulkan.hpp
)
source_group("inc" FILES ${INC})

if(LUG_OS_ANDROID)
    list(APPEND EXT_LIBRARIES android)
endif()

# Find glTF2-loader
find_package(GLTF2-Loader)

if (NOT GLTF2-LOADER_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/glTF2-loader")
        message(FATAL_ERROR "Can't find glTF2-loader in the thirdparty directory")
    endif()

    set(GLTF2-LOADER_ROOT "${LUG_THIRDPARTY_DIR}/glTF2-loader")

    find_package(GLTF2-Loader REQUIRED)

    message(STATUS "Found glTF2-loader library: ${GLTF2-LOADER_LIBRARY}")
    message(STATUS "Found glTF2-loader includes: ${GLTF2-LOADER_INCLUDE_DIR}")
endif()

include_directories(${GLTF2-LOADER_INCLUDE_DIR})

find_package(Threads)

# find imgui
if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/imgui")
    message(FATAL_ERROR "Can't find imgui in the thirdparty directory")
endif()

set(IMGUI_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/imgui/include)
message(STATUS "Found Imgui: ${IMGUI_INCLUDE_DIR}")

set(IMGUI_SOURCE_DIR ${LUG_THIRDPARTY_DIR}/imgui/src)
message(STATUS "Found Imgui: ${IMGUI_SOURCE_DIR}")

include_directories(${IMGUI_INCLUDE_DIR})

set(IMGUI_SOURCE_FILES
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
)

# find imgui_club
if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/imgui_club")
    message(FATAL_ERROR "Can't find imgui_club in the thirdparty directory")
endif()

set(IMGUI_CLUB_DIR ${LUG_THIRDPARTY_DIR}/imgui_club)
message(STATUS "Found Imgui_club: ${IMGUI_CLUB_DIR}")

include_directories(${IMGUI_CLUB_DIR}/include)
list(APPEND IMGUI_SOURCE_FILES ${IMGUI_CLUB_DIR}/src/imgui_freetype.cpp)

# find freetype
find_package(Freetype)
if (NOT FREETYPE_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/freetype")
        message(FATAL_ERROR "Can't find freetype in the thirdparty directory")
    endif()

    if(LUG_OS_ANDROID)
        set(CMAKE_FIND_ROOT_PATH "${LUG_THIRDPARTY_DIR}/freetype")
    else()
        set(ENV{FREETYPE_DIR} "${LUG_THIRDPARTY_DIR}/freetype")
    endif()
    find_package(Freetype REQUIRED)

    message(STATUS "Found freetype include : ${FREETYPE_INCLUDE_DIRS}")
    message(STATUS "Found freetype library : ${FREETYPE_LIBRARIES}")
endif()

include_directories(${FREETYPE_INCLUDE_DIRS})

if(LUG_COMPILER_GCC)
    set_source_files_properties(${IMGUI_SOURCE_FILES} PROPERTIES COMPILE_FLAGS -Wno-error=maybe-uninitialized)
endif()

# define the target
lug_add_library(lug-graphics
                SOURCES ${SRC} ${INC} ${IMGUI_SOURCE_FILES}
                DEPENDS lug-window lug-system lug-math
                EXTERNAL_LIBS ${EXT_LIBRARIES} ${GLTF2-LOADER_LIBRARY} ${SHADERC_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${FREETYPE_LIBRARIES}
)
