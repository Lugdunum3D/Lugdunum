#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class CommandPool;
} // Builder

class Device;
class Queue;

class LUG_GRAPHICS_API CommandPool {
    friend class Builder::CommandPool;

public:
    CommandPool() = default;

    CommandPool(const CommandPool&) = delete;
    // Warning: Don't move CommandPool after creating a CommandBuffer
    CommandPool(CommandPool&& CommandPool);

    CommandPool& operator=(const CommandPool&) = delete;
    // Warning: Don't move CommandPool after creating a CommandBuffer
    CommandPool& operator=(CommandPool&& CommandPool);

    ~CommandPool();

    explicit operator VkCommandPool() const {
        return _commandPool;
    }

    const Device* getDevice() const;
    const Queue* getQueue() const;

    bool reset(bool releaseRessources = false) const;

    // Warning: Command buffers created with this pool must have been destroyed
    void destroy();

private:
    explicit CommandPool(VkCommandPool commandPool, const Device *device, const Queue *queue);

private:
    VkCommandPool _commandPool{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    const Queue* _queue{nullptr};
};

#include <lug/Graphics/Vulkan/API/CommandPool.inl>

} // API
} // Vulkan
} // Graphics
} // lug
