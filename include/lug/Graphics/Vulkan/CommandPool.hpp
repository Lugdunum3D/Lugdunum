#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Queue;

class LUG_GRAPHICS_API CommandPool {
public:
    CommandPool(VkCommandPool commandPool = VK_NULL_HANDLE, Device *device = nullptr, Queue *queue = nullptr);

    CommandPool(const CommandPool&) = delete;
    CommandPool(CommandPool&& CommandPool);

    CommandPool& operator=(const CommandPool&) = delete;
    CommandPool& operator=(CommandPool&& CommandPool);

    ~CommandPool();

    operator VkCommandPool() const {
        return _commandPool;
    }

    void destroy();

private:
    VkCommandPool _commandPool{VK_NULL_HANDLE};
    Device* _device{nullptr};
    Queue* _queue{nullptr};
};

} // Vulkan
} // Graphics
} // lug
