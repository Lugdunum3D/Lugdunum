#include <lug/Graphics/Vulkan/Queue.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Debug.hpp>

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

bool Queue::submit(VkCommandBuffer commandBuffer,
                   const std::vector<VkSemaphore>& signalSemaphores,
                   const std::vector<VkSemaphore>& waitSemaphores,
                   const std::vector<VkPipelineStageFlags>& waitDstStageMasks,
                   VkFence fence) const {

    LUG_ASSERT(waitSemaphores.size() == waitDstStageMasks.size(), "waitDstStageMasks should be the same size as waitSemaphores");

    VkSubmitInfo submitInfo{
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        submitInfo.pNext = nullptr,
        submitInfo.waitSemaphoreCount = waitSemaphores.size() > 0 ? (uint32_t)waitSemaphores.size() : 0,
        submitInfo.pWaitSemaphores = waitSemaphores.size() > 0 ? waitSemaphores.data() : nullptr,
        submitInfo.pWaitDstStageMask = waitDstStageMasks.size() > 0 ? waitDstStageMasks.data() : nullptr,
        submitInfo.commandBufferCount = 1,
        submitInfo.pCommandBuffers = &commandBuffer,
        submitInfo.signalSemaphoreCount = signalSemaphores.size() > 0 ? (uint32_t)signalSemaphores.size() : 0,
        submitInfo.pSignalSemaphores = signalSemaphores.size() > 0 ? signalSemaphores.data() : nullptr
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
