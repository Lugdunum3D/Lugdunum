#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Buffer;
class Device;
class Image;

namespace Builder {

class DeviceMemory {
public:
    DeviceMemory(const API::Device& deviceMemory);

    DeviceMemory(const DeviceMemory&&) = delete;
    DeviceMemory(DeviceMemory&&) = delete;

    DeviceMemory& operator=(const DeviceMemory&&) = delete;
    DeviceMemory& operator=(DeviceMemory&&) = delete;

    ~DeviceMemory() = default;

    // Setters
    void setMemoryFlags(VkMemoryPropertyFlags flags);

    bool addBuffer(API::Buffer& buffer);
    bool addImage(API::Image& image);

    // Build methods
    bool build(API::DeviceMemory& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::DeviceMemory> build(VkResult* returnResult = nullptr);

public:
    static uint32_t findMemoryType(const API::Device& device, uint32_t memoryTypeBits, VkMemoryPropertyFlags requiredFlags);

private:
    const API::Device& _device;

    VkMemoryPropertyFlags _memoryFlags{VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT};
    uint32_t _memoryTypeBits{0xFFFFFFFF};

    std::vector<API::Buffer*> _buffers;
    std::vector<API::Image*> _images;
};

#include <lug/Graphics/Vulkan/API/Builder/DeviceMemory.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
