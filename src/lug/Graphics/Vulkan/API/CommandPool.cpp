#include <lug/Graphics/Vulkan/API/CommandPool.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

CommandPool::CommandPool(VkCommandPool commandPool, const Device* device, const Queue* queue) : _commandPool(commandPool), _device(device), _queue(queue) {}

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

bool CommandPool::reset(bool releaseRessources) const {
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

} // API
} // Vulkan
} // Graphics
} // lug
