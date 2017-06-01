#pragma once

#include <memory>

#include <lug/Graphics/Vulkan/API/CommandPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;
class QueueFamily;

namespace Builder {

class CommandPool {
public:
    CommandPool(const API::Device& device, const API::QueueFamily& queueFamily);

    CommandPool(const CommandPool&&) = delete;
    CommandPool(CommandPool&&) = delete;

    CommandPool& operator=(const CommandPool&&) = delete;
    CommandPool& operator=(CommandPool&&) = delete;

    ~CommandPool() = default;

    // Setters
    void setFlags(VkCommandPoolCreateFlags flags);

    // Build methods
    bool build(API::CommandPool& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::CommandPool> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    const API::QueueFamily& _queueFamily;
    VkCommandPoolCreateFlags _flags{VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT};
};

#include <lug/Graphics/Vulkan/API/Builder/CommandPool.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
