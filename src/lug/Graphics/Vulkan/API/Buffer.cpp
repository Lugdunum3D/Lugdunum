#include <lug/Graphics/Vulkan/API/Buffer.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/System/Logger/Logger.hpp>

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

void Buffer::bindMemory(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset) {
    _deviceMemory = deviceMemory;
    _deviceMemoryOffset = memoryOffset;
    vkBindBufferMemory(static_cast<VkDevice>(*_device), static_cast<VkBuffer>(_buffer), static_cast<VkDeviceMemory>(*deviceMemory), memoryOffset);
}

void* Buffer::mapMemory(VkDeviceSize size) {
    void* data = nullptr;

    vkMapMemory(static_cast<VkDevice>(*_device), static_cast<VkDeviceMemory>(*_deviceMemory), _deviceMemoryOffset, size, 0, &data);

    return data;
}

void Buffer::unmapMemory() {
    vkUnmapMemory(static_cast<VkDevice>(*_device), static_cast<VkDeviceMemory>(*_deviceMemory));
}

void Buffer::updateData(void* data, uint32_t size) {
    void* gpuData = mapMemory(size);

    memcpy(gpuData, data, size);
    unmapMemory();
}

void Buffer::updateDataTransfer(const CommandBuffer* commandBuffer, void* data, uint32_t size, uint32_t offset) {
    vkCmdUpdateBuffer(static_cast<VkCommandBuffer>(*commandBuffer), _buffer, offset, size, data);
}

const VkMemoryRequirements& Buffer::getRequirements() const {
    return _requirements;
}

uint32_t Buffer::getSizeAligned(const Device* device, uint32_t size) {
    uint32_t alignment = (uint32_t)device->getPhysicalDeviceInfo()->properties.limits.minUniformBufferOffsetAlignment;
    uint32_t sizeAligned = 0;

    if (size % alignment != 0) {
        sizeAligned = static_cast<uint32_t>(size + alignment - (size % alignment));
    } else {
        sizeAligned = static_cast<uint32_t>(size);
    }

    return sizeAligned;
}

} // API
} // Vulkan
} // Graphics
} // lug
