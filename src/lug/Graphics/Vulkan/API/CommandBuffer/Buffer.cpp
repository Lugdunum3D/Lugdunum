#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

#include <lug/Graphics/Vulkan/API/Buffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

void CommandBuffer::updateBuffer(const API::Buffer& buffer, const void* data, VkDeviceSize size, VkDeviceSize offset) const {
    vkCmdUpdateBuffer(_commandBuffer, static_cast<VkBuffer>(buffer), offset, size, data);
}

} // API
} // Vulkan
} // Graphics
} // lug
