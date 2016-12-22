#include <lug/Graphics/Vulkan/CommandPool.hpp>
#include <lug/System/Debug.hpp>

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

void CommandPool::destroy() {
    if (_commandPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(*_device, _commandPool, nullptr);
        _commandPool = VK_NULL_HANDLE;
    }

    _device = nullptr;
    _queue = nullptr;
}

} // Vulkan
} // Graphics
} // lug
