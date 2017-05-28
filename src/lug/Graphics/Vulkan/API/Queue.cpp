#include <lug/Graphics/Vulkan/API/Queue.hpp>

#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/System/Debug.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Queue::Queue(VkQueue queue) : _queue(queue) {}

Queue::Queue(Queue&& queue) {
    _queue = queue._queue;

    queue._queue = VK_NULL_HANDLE;
}

Queue& Queue::operator=(Queue&& queue) {
    _queue = queue._queue;

    queue._queue = VK_NULL_HANDLE;

    return *this;
}

bool Queue::submit(
    const CommandBuffer& commandBuffer,
    const std::vector<VkSemaphore>& signalSemaphores,
    const std::vector<VkSemaphore>& waitSemaphores,
    const std::vector<VkPipelineStageFlags>& waitDstStageMasks,
    VkFence fence) const {

    LUG_ASSERT(waitSemaphores.size() == waitDstStageMasks.size(), "waitDstStageMasks should be the same size as waitSemaphores");

    VkCommandBuffer vkCommandBuffer = static_cast<VkCommandBuffer>(commandBuffer);

    const VkSubmitInfo submitInfo{
        /* submitInfo.sType */ VK_STRUCTURE_TYPE_SUBMIT_INFO,
        /* submitInfo.pNext */ nullptr,
        /* submitInfo.waitSemaphoreCount */ waitSemaphores.size() > 0 ? (uint32_t)waitSemaphores.size() : 0,
        /* submitInfo.pWaitSemaphores */ waitSemaphores.size() > 0 ? waitSemaphores.data() : nullptr,
        /* submitInfo.pWaitDstStageMask */ waitDstStageMasks.size() > 0 ? waitDstStageMasks.data() : nullptr,
        /* submitInfo.commandBufferCount */ 1,
        /* submitInfo.pCommandBuffers */ &vkCommandBuffer,
        /* submitInfo.signalSemaphoreCount */ signalSemaphores.size() > 0 ? (uint32_t)signalSemaphores.size() : 0,
        /* submitInfo.pSignalSemaphores */ signalSemaphores.size() > 0 ? signalSemaphores.data() : nullptr
    };

    VkResult result = vkQueueSubmit(_queue, 1, &submitInfo, fence);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("Queue::submit: Can't submit command buffer {}", result);
        return false;
    }

    return true;
}

bool Queue::waitIdle() const {
    VkResult result = vkQueueWaitIdle(_queue);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("Queue::waitIdle: Can't wait for queue work {}", result);
        return false;
    }

    return true;
}

} // API
} // Vulkan
} // Graphics
} // lug
