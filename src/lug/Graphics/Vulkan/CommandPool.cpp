#include <lug/Graphics/Vulkan/CommandPool.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

CommandPool::CommandPool(VkCommandPool commandPool, Device* device, Queue* queue) : _commandPool(commandPool), _device(device), _queue(queue) {}

CommandPool::CommandPool(CommandPool&& commandPool) {
    _commandPool = commandPool._commandPool;
    _device = commandPool._device;
    _queue = commandPool._queue;

    commandPool._commandPool = VK_NULL_HANDLE;
    commandPool._device = nullptr;
    commandPool._queue = nullptr;
}

CommandPool& CommandPool::operator=(CommandPool&& commandPool) {
    destroy();

    _commandPool = commandPool._commandPool;
    _device = commandPool._device;
    _queue = commandPool._queue;

    commandPool._commandPool = VK_NULL_HANDLE;
    commandPool._device = nullptr;
    commandPool._queue = nullptr;

    return *this;
}

CommandPool::~CommandPool() {
    destroy();
}

std::vector<CommandBuffer> CommandPool::createCommandBuffers(VkCommandBufferLevel level, uint32_t count) {
    VkCommandBufferAllocateInfo allocateInfo{
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        allocateInfo.pNext = nullptr,
        allocateInfo.commandPool = _commandPool,
        allocateInfo.level = level,
        allocateInfo.commandBufferCount = count
    };

    std::vector<VkCommandBuffer> commandBuffers(count);
    VkResult result = vkAllocateCommandBuffers(static_cast<VkDevice>(*_device), &allocateInfo, commandBuffers.data());
    if (result != VK_SUCCESS) {
        LUG_LOG.error("CommandPool: Can't allocate command buffers: {}", result);
        return {};
    }

    std::vector<CommandBuffer> commandBuffersReturn(count);

    for (uint32_t i = 0; i < count; ++i) {
        commandBuffersReturn[i] = CommandBuffer(commandBuffers[i], this);
    }

    return commandBuffersReturn;
}

bool CommandPool::reset(bool releaseRessources) {
    VkResult result = vkResetCommandPool(static_cast<VkDevice>(*_device), _commandPool, releaseRessources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("CommandPool: Can't reset the pool: {}", result);
        return false;
    }

    return true;
}

void CommandPool::destroy() {
    if (_commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(static_cast<VkDevice>(*_device), _commandPool, nullptr);
        _commandPool = VK_NULL_HANDLE;
    }

    _device = nullptr;
    _queue = nullptr;
}

} // Vulkan
} // Graphics
} // lug
