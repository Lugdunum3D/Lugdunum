#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

#include <lug/Graphics/Vulkan/API/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

CommandBuffer::CommandBuffer(VkCommandBuffer commandBuffer, const CommandPool* commandPool) : _commandBuffer(commandBuffer), _commandPool(commandPool) {}

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

bool CommandBuffer::begin(VkCommandBufferUsageFlags flags) const {
    VkCommandBufferBeginInfo beginInfo{
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        beginInfo.pNext = nullptr,
        beginInfo.flags = flags,
        beginInfo.pInheritanceInfo = nullptr // TODO: handle pInheritanceInfo for secondary command buffers
    };

    VkResult result = vkBeginCommandBuffer(_commandBuffer, &beginInfo);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("CommandBuffer: Can't begin the command buffer: {}", result);
        return false;
    }

    return true;
}

bool CommandBuffer::end() const {
    VkResult result = vkEndCommandBuffer(_commandBuffer);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("CommandBuffer: Can't end the command buffer: {}", result);
        return false;
    }

    return true;
}

bool CommandBuffer::reset(bool releaseRessources) const {
    VkResult result = vkResetCommandBuffer(_commandBuffer, releaseRessources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("CommandBuffer: Can't reset the command buffer: {}", result);
        return false;
    }

    return true;
}

void CommandBuffer::destroy() {
    if (_commandBuffer != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(static_cast<VkDevice>(*_commandPool->getDevice()), static_cast<VkCommandPool>(*_commandPool), 1, &_commandBuffer);
        _commandBuffer = VK_NULL_HANDLE;
    }

    _commandPool = nullptr;
}

} // API
} // Vulkan
} // Graphics
} // lug
