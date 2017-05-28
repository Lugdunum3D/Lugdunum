#include <lug/Graphics/Vulkan/API/Buffer.hpp>

#include <cstring>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Buffer::Buffer(
    VkBuffer buffer,
    const Device* device) : _buffer(buffer), _device(device) {
    vkGetBufferMemoryRequirements(static_cast<VkDevice>(*device), _buffer, &_requirements);
}

Buffer::Buffer(Buffer&& buffer) {
    _buffer = buffer._buffer;
    _device = buffer._device;
    _deviceMemory = buffer._deviceMemory;
    _requirements = buffer._requirements;
    _deviceMemoryOffset = buffer._deviceMemoryOffset;
    buffer._buffer = VK_NULL_HANDLE;
    buffer._device = nullptr;
    buffer._deviceMemory = nullptr;
    buffer._requirements = {};
    buffer._deviceMemoryOffset = 0;
}

Buffer& Buffer::operator=(Buffer&& buffer) {
    destroy();

    _buffer = buffer._buffer;
    _device = buffer._device;
    _deviceMemory = buffer._deviceMemory;
    _requirements = buffer._requirements;
    _deviceMemoryOffset = buffer._deviceMemoryOffset;
    buffer._buffer = VK_NULL_HANDLE;
    buffer._device = nullptr;
    buffer._deviceMemory = nullptr;
    buffer._requirements = {};
    buffer._deviceMemoryOffset = 0;

    return *this;
}

Buffer::~Buffer() {
    destroy();
}

void Buffer::destroy() {
    if (_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(static_cast<VkDevice>(*_device), _buffer, nullptr);
        _buffer = VK_NULL_HANDLE;
    }
}

void Buffer::bindMemory(const DeviceMemory& deviceMemory, VkDeviceSize memoryOffset) {
    _deviceMemory = &deviceMemory;
    _deviceMemoryOffset = memoryOffset;

    vkBindBufferMemory(static_cast<VkDevice>(*_device), static_cast<VkBuffer>(_buffer), static_cast<VkDeviceMemory>(deviceMemory), memoryOffset);
}

bool Buffer::updateData(const void* data, VkDeviceSize size, VkDeviceSize offset) const {
    void* gpuData = _deviceMemory->mapBuffer(*this, size, offset);

    if (!gpuData) {
        return false;
    }

    memcpy(gpuData, data, size);
    _deviceMemory->unmap();

    return true;
}

} // API
} // Vulkan
} // Graphics
} // lug
