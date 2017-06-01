#pragma once

#include <memory>
#include <set>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class Buffer {
public:
    Buffer(const API::Device& device);

    Buffer(const Buffer&&) = delete;
    Buffer(Buffer&&) = delete;

    Buffer& operator=(const Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    ~Buffer() = default;

    // Setters
    void setSize(VkDeviceSize size);
    void setFlags(VkBufferCreateFlags flags);
    void setUsage(VkBufferUsageFlags usage);
    void setExclusive(bool exclusive);
    void setQueueFamilyIndices(std::set<uint32_t> queueFamilyIndices);

    // Build methods
    bool build(API::Buffer& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Buffer> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;
    VkDeviceSize _size{};
    VkBufferCreateFlags _flags{};
    bool _exclusive{false};
    VkBufferUsageFlags _usage{};
    std::set<uint32_t> _queueFamilyIndices{};
};

#include <lug/Graphics/Vulkan/API/Builder/Buffer.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
