#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class CommandBuffer;
class Device;
class DeviceMemory;

class LUG_GRAPHICS_API Buffer {
public:
    explicit Buffer(VkBuffer Buffer = VK_NULL_HANDLE, const Device* device = nullptr, DeviceMemory* deviceMemory = nullptr);

    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& buffer);

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& buffer);

    ~Buffer();

    explicit operator VkBuffer() const {
        return _buffer;
    }

    void destroy();

    void bindMemory(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset = 0);
    void* mapMemory(VkDeviceSize size, VkDeviceSize offset = 0);
    void unmapMemory();
    void updateData(void *data, uint32_t size, uint32_t memoryOffset = 0);
    void updateDataTransfer(const CommandBuffer* commandBuffer, void *data, uint32_t size, uint32_t offset = 0);

    static uint32_t getSizeAligned(const Device* device, uint32_t size);

    const VkMemoryRequirements& getRequirements() const;

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
    DeviceMemory* _deviceMemory{nullptr};

    VkMemoryRequirements _requirements{};
};

} // Vulkan
} // Graphics
} // lug
