#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Buffer::Buffer(VkBuffer buffer,
    const Device* device,
    DeviceMemory* deviceMemory) : _buffer(buffer), _device(device), _deviceMemory(deviceMemory) {
    vkGetBufferMemoryRequirements(*device, _buffer, &_requirements);
}

Buffer::Buffer(Buffer&& buffer) {
    _buffer = buffer._buffer;
    _device = buffer._device;
    _deviceMemory = buffer._deviceMemory;
    _requirements = buffer._requirements;
    buffer._buffer = VK_NULL_HANDLE;
    buffer._device = nullptr;
    buffer._deviceMemory = nullptr;
    buffer._requirements = {};
}

Buffer& Buffer::operator=(Buffer&& buffer) {
    destroy();

    _buffer = buffer._buffer;
    _device = buffer._device;
    _deviceMemory = buffer._deviceMemory;
    _requirements = buffer._requirements;
    buffer._buffer = VK_NULL_HANDLE;
    buffer._device = nullptr;
    buffer._deviceMemory = nullptr;
    buffer._requirements = {};

    return *this;
}

Buffer::~Buffer() {
    destroy();
}

void Buffer::destroy() {
    if (_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(*_device, _buffer, nullptr);
        _buffer = VK_NULL_HANDLE;
    }
}

void Buffer::bindMemory(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset) {
    _deviceMemory = deviceMemory;
    vkBindBufferMemory(*_device, _buffer, *deviceMemory, memoryOffset);
}

void* Buffer::mapMemory(VkDeviceSize size, VkDeviceSize offset) {
    void* data = nullptr;
    vkMapMemory(*_device, *_deviceMemory, offset, size, 0, &data);

    return data;
}

void Buffer::unmapMemory() {
    vkUnmapMemory(*_device, *_deviceMemory);
}

void Buffer::updateData(void *data, uint32_t size, uint32_t memoryOffset) {
    void *gpuData = mapMemory(size, memoryOffset);
    memcpy(gpuData, data, size);
    unmapMemory();
}

void Buffer::updateDataTransfer(const CommandBuffer* commandBuffer, void *data, uint32_t size, uint32_t offset) {
    vkCmdUpdateBuffer(*commandBuffer, _buffer, offset, size, data);
}

const VkMemoryRequirements& Buffer::getRequirements() const {
    return _requirements;
}

std::unique_ptr<Buffer> Buffer::create(
        const Device* device,
        uint32_t queueFamilyIndexCount,
        const uint32_t* pQueueFamilyIndices,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkBufferCreateFlags createFlags,
        VkSharingMode sharingMode
) {
    // Create buffer
    VkBufferCreateInfo createInfo {
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = createFlags,
        createInfo.size = size,
        createInfo.usage = usage,
        createInfo.sharingMode = sharingMode,
        createInfo.queueFamilyIndexCount = queueFamilyIndexCount,
        createInfo.pQueueFamilyIndices = pQueueFamilyIndices
    };

    VkBuffer bufferHandle = VK_NULL_HANDLE;
    VkResult result = vkCreateBuffer(*device, &createInfo, nullptr, &bufferHandle);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create buffer: {}", result);
        return nullptr;
    }

    return std::unique_ptr<Buffer>(new Buffer(bufferHandle, device));
}

} // Vulkan
} // Graphics
} // lug
