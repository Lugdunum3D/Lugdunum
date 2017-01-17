#include <lug/Graphics/Vulkan/Buffer.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Buffer::Buffer(VkBuffer buffer,
    const Device* device,
    std::unique_ptr<DeviceMemory> deviceMemory) : _buffer(buffer), _device(device), _deviceMemory(std::move(deviceMemory)) {}

Buffer::Buffer(Buffer&& buffer) {
    _buffer = buffer._buffer;
    _device = buffer._device;
    _deviceMemory = std::move(buffer._deviceMemory);
    buffer._buffer = VK_NULL_HANDLE;
    buffer._device = nullptr;
    buffer._deviceMemory = nullptr;
}

Buffer& Buffer::operator=(Buffer&& buffer) {
    _buffer = buffer._buffer;
    _device = buffer._device;
    _deviceMemory = std::move(buffer._deviceMemory);
    buffer._buffer = VK_NULL_HANDLE;
    buffer._device = nullptr;
    buffer._deviceMemory = nullptr;

    return *this;
}

Buffer::~Buffer() {
    destroy();
}

void Buffer::destroy() {
    if (_deviceMemory != nullptr) {
        _deviceMemory->destroy();
        _deviceMemory = nullptr;
    }

    if (_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(*_device, _buffer, nullptr);
        _buffer = VK_NULL_HANDLE;
    }
}

void Buffer::bindMemory(const DeviceMemory& deviceMemory, VkDeviceSize memoryOffset) {
    vkBindBufferMemory(*_device, _buffer, deviceMemory, memoryOffset);
}

void* Buffer::mapMemory(VkDeviceSize size, VkDeviceSize offset) {
    void* data = nullptr;
    vkMapMemory(*_device, *_deviceMemory, offset, size, 0, &data);

    return data;
}

void Buffer::unmapMemory() {
    vkUnmapMemory(*_device, *_deviceMemory);
}

void Buffer::updateData(void *data, uint32_t size, VkDeviceSize memoryOffset) {
    void *gpuData = mapMemory(size, memoryOffset);
    memcpy(gpuData, data, size);
    unmapMemory();
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


    // Allocate device memory for buffer
    VkMemoryRequirements memoryRequirements{};
    vkGetBufferMemoryRequirements(*device, bufferHandle, &memoryRequirements);

    VkDeviceSize memoryTypeIndex = DeviceMemory::findMemoryType(device, memoryRequirements);
    auto deviceMemory = DeviceMemory::allocate(device, memoryRequirements.size, memoryTypeIndex);

    auto buffer = std::unique_ptr<Buffer>(new Buffer(bufferHandle, device, std::move(deviceMemory)));
    buffer->bindMemory(*deviceMemory);

    return buffer;
}

} // Vulkan
} // Graphics
} // lug
