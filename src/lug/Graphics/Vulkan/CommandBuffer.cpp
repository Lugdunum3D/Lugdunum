#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/CommandPool.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

CommandBuffer::CommandBuffer(VkCommandBuffer commandBuffer, CommandPool* commandPool) : _commandBuffer(commandBuffer), _commandPool(commandPool) {}

CommandBuffer::CommandBuffer(CommandBuffer&& commandBuffer) {
    _commandBuffer = commandBuffer._commandBuffer;
    _commandPool = commandBuffer._commandPool;

    commandBuffer._commandBuffer = VK_NULL_HANDLE;
    commandBuffer._commandPool = nullptr;
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& commandBuffer) {
    destroy();

    _commandBuffer = commandBuffer._commandBuffer;
    _commandPool = commandBuffer._commandPool;

    commandBuffer._commandBuffer = VK_NULL_HANDLE;
    commandBuffer._commandPool = nullptr;

    return *this;
}

CommandBuffer::~CommandBuffer() {
    destroy();
}

bool CommandBuffer::reset(bool releaseRessources) {
    VkResult result = vkResetCommandBuffer(_commandBuffer, releaseRessources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("CommandBuffer: Can't reset the command buffer: {}", result);
        return false;
    }

    return true;
}

void CommandBuffer::destroy() {
    if (_commandBuffer != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(*_commandPool->getDevice(), *_commandPool, 1, &_commandBuffer);
        _commandBuffer = VK_NULL_HANDLE;
    }

    _commandPool = nullptr;
}

} // Vulkan
} // Graphics
} // lug
