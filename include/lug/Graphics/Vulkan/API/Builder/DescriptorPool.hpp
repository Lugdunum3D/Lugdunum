#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class DescriptorPool {
public:
    DescriptorPool(const API::Device& device);

    DescriptorPool(const DescriptorPool&&) = delete;
    DescriptorPool(DescriptorPool&&) = delete;

    DescriptorPool& operator=(const DescriptorPool&&) = delete;
    DescriptorPool& operator=(DescriptorPool&&) = delete;

    ~DescriptorPool() = default;

    // Setters
    void setFlags(VkDescriptorPoolCreateFlags flags);
    void setMaxSets(uint32_t maxSets);
    void setPoolSizes(const std::vector<VkDescriptorPoolSize>& poolSizes);

    // Build methods
    bool build(API::DescriptorPool& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::DescriptorPool> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    VkDescriptorPoolCreateFlags _flags{VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT};
    uint32_t _maxSets{0};
    std::vector<VkDescriptorPoolSize> _poolSizes{};
};

#include <lug/Graphics/Vulkan/API/Builder/DescriptorPool.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
