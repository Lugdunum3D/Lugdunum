#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <memory>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Buffer {
public:
    Buffer(VkBuffer Buffer = VK_NULL_HANDLE, const Device* device = nullptr, std::unique_ptr<DeviceMemory> deviceMemory = nullptr);

    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& buffer);

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& buffer);

    ~Buffer();

    operator VkBuffer() const {
        return _buffer;
    }

    void destroy();

    void bindMemory(const DeviceMemory& deviceMemory, VkDeviceSize memoryOffset = 0);
    void* mapMemory(VkDeviceSize size, VkDeviceSize offset = 0);
    void unmapMemory();
    void updateData(void *data, uint32_t size, VkDeviceSize memoryOffset = 0);

    static std::unique_ptr<Buffer> create(
        const Device* device,
        uint32_t queueFamilyIndexCount,
        const uint32_t* pQueueFamilyIndices,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkBufferCreateFlags createFlags = 0,
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE
    );

private:
    VkBuffer _buffer{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::unique_ptr<DeviceMemory> _deviceMemory{nullptr};
};

} // Vulkan
} // Graphics
} // lug
