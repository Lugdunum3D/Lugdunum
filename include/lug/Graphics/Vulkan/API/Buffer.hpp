#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandBuffer;
class Device;
class DeviceMemory;

namespace Builder {
class Buffer;
}

class LUG_GRAPHICS_API Buffer {
    friend class Builder::Buffer;

public:
    Buffer() = default;

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

    bool updateData(void *data, VkDeviceSize size, VkDeviceSize offset = 0);
    void updateDataTransfer(const CommandBuffer* commandBuffer, void *data, uint32_t size, uint32_t offset = 0);

    const VkMemoryRequirements& getRequirements() const;
    DeviceMemory* getDeviceMemory() const;
    VkDeviceSize getDeviceMemoryOffset() const;

private:
    explicit Buffer(VkBuffer Buffer, const Device* device);

private:
    VkBuffer _buffer{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    DeviceMemory* _deviceMemory{nullptr};
    VkDeviceSize _deviceMemoryOffset{0};

    VkMemoryRequirements _requirements{};
};

#include <lug/Graphics/Vulkan/API/Buffer.inl>

} // API
} // Vulkan
} // Graphics
} // lug
