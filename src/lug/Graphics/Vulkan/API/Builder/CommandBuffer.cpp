#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/CommandPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

CommandBuffer::CommandBuffer(const API::Device& device, const API::CommandPool& commandPool) : _device{device}, _commandPool(commandPool) {}

bool CommandBuffer::build(API::CommandBuffer& instance, VkResult* returnResult) {
    std::vector<API::CommandBuffer> descriptorSets(1);
    if (!build(descriptorSets, returnResult)) {
        return false;
    }
    instance = std::move(descriptorSets[0]);
    return true;
}

std::unique_ptr<API::CommandBuffer> CommandBuffer::build(VkResult* returnResult) {
    std::vector<API::CommandBuffer> commandBuffers(1);
    return build(commandBuffers, returnResult) ? std::make_unique<API::CommandBuffer>(std::move(commandBuffers[0])) : nullptr;
}

bool CommandBuffer::build(std::vector<API::CommandBuffer>& commandBuffers, VkResult* returnResult) {
    // Create the commandBuffer creation information for vkCreateCommandBuffer
    const VkCommandBufferAllocateInfo allocateInfo{
        /* allocateInfo.sType */ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        /* allocateInfo.pNext */ nullptr,
        /* allocateInfo.commandPool */ static_cast<VkCommandPool>(_commandPool),
        /* allocateInfo.level */ _level,
        /* allocateInfo.commandBufferCount */ static_cast<uint32_t>(commandBuffers.size()),
    };

    std::vector<VkCommandBuffer> vkCommandBuffers(commandBuffers.size());

    // Create the commandBuffers
    VkResult result = vkAllocateCommandBuffers(static_cast<VkDevice>(_device), &allocateInfo, vkCommandBuffers.data());

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    for (uint32_t i = 0; i < commandBuffers.size(); ++i) {
        commandBuffers[i] = API::CommandBuffer(vkCommandBuffers[i], &_commandPool);
    }

    return true;
}

std::vector<std::unique_ptr<API::CommandBuffer>> CommandBuffer::build(uint32_t count, VkResult* returnResult) {
    // Output vector
    std::vector<API::CommandBuffer> commandBuffers(count);
    std::vector<std::unique_ptr<API::CommandBuffer>> outCommandBuffers(count);

    // Build the command buffers
    if (!build(commandBuffers, returnResult)) {
        return {};
    }

    // Initialize the unique_ptr array
    for (uint32_t i = 0; i < count; ++i) {
        outCommandBuffers[i] = std::make_unique<API::CommandBuffer>(std::move(commandBuffers[i]));
    }

    return outCommandBuffers;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
