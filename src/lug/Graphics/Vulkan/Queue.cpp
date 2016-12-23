#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Queue::Queue(int8_t idx, VkQueue queue, VkQueueFlags flags, bool presentation) : _idx(idx), _queue(queue), _flags(flags), _presentation(presentation) {}

Queue::Queue(Queue&& queue) {
    _queue = queue._queue;
    _idx = queue._idx;
    _flags = queue._flags;
    _presentation = queue._presentation;
    _commandPool = std::move(queue._commandPool);

    queue._queue = VK_NULL_HANDLE;
    queue._idx = -1;
    queue._flags = 0;
    queue._presentation = false;
}

Queue& Queue::operator=(Queue&& queue) {
    destroy();

    _queue = queue._queue;
    _idx = queue._idx;
    _flags = queue._flags;
    _presentation = queue._presentation;
    _commandPool = std::move(queue._commandPool);

    queue._queue = VK_NULL_HANDLE;
    queue._idx = -1;
    queue._flags = 0;
    queue._presentation = false;

    return *this;
}

Queue::~Queue() {
    destroy();
}

bool Queue::submit(VkCommandBuffer commandBuffer, VkSemaphore signalSemaphore, VkSemaphore waitSemaphore, VkPipelineStageFlags waitDstStageMask, VkFence fence) const {
    VkSubmitInfo submitInfo{
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        submitInfo.pNext = nullptr,
        submitInfo.waitSemaphoreCount = waitSemaphore != VK_NULL_HANDLE ? 1 : 0,
        submitInfo.pWaitSemaphores = waitSemaphore != VK_NULL_HANDLE ? &waitSemaphore : nullptr,
        submitInfo.pWaitDstStageMask = waitSemaphore != VK_NULL_HANDLE ? &waitDstStageMask : nullptr,
        submitInfo.commandBufferCount = 1,
        submitInfo.pCommandBuffers = &commandBuffer,
        submitInfo.signalSemaphoreCount = signalSemaphore != VK_NULL_HANDLE ? 1 : 0,
        submitInfo.pSignalSemaphores = signalSemaphore != VK_NULL_HANDLE ? &signalSemaphore : nullptr
    };

    VkResult result = vkQueueSubmit(_queue, 1, &submitInfo, fence);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't enumerate instance layers: {}", result);
        return false;
    }

    return true;
}

bool Queue::waitIdle() const {
    VkResult result = vkQueueWaitIdle(_queue);
    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't enumerate instance layers: {}", result);
        return false;
    }

    return true;
}

void Queue::destroy() {
    _commandPool.destroy();

    if (_queue != VK_NULL_HANDLE) {
        waitIdle();
        _queue = VK_NULL_HANDLE;
    }

    _idx = -1;
    _flags = 0;
    _presentation = false;
}

} // Vulkan
} // Graphics
} // lug
