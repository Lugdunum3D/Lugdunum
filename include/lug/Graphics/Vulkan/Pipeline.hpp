#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Pipeline {
public:
    explicit Pipeline(VkPipeline pipeline = VK_NULL_HANDLE, const Device* device = nullptr);

    Pipeline(const Pipeline&) = delete;
    Pipeline(Pipeline&& device);

    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline& operator=(Pipeline&& device);

    ~Pipeline();

    operator VkPipeline() const {
        return _pipeline;
    }

    void bind(const CommandBuffer* commandBuffer);

    void destroy();

private:
    VkPipeline _pipeline{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
