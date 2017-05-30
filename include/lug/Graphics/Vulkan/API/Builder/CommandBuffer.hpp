#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandPool;
class Device;

namespace Builder {

class CommandBuffer {
public:
    CommandBuffer(const API::Device& device, const API::CommandPool& commandPool);

    CommandBuffer(const CommandBuffer&&) = delete;
    CommandBuffer(CommandBuffer&&) = delete;

    CommandBuffer& operator=(const CommandBuffer&&) = delete;
    CommandBuffer& operator=(CommandBuffer&&) = delete;

    ~CommandBuffer() = default;

    // Setters
    void setLevel(VkCommandBufferLevel level);

    // Build methods
    bool build(API::CommandBuffer& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::CommandBuffer> build(VkResult* returnResult = nullptr);

    bool build(std::vector<API::CommandBuffer>& commandBuffers, VkResult* returnResult = nullptr);
    std::vector<std::unique_ptr<API::CommandBuffer>> build(uint32_t count, VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    const API::CommandPool& _commandPool;
    VkCommandBufferLevel _level{VK_COMMAND_BUFFER_LEVEL_PRIMARY};
};

#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
