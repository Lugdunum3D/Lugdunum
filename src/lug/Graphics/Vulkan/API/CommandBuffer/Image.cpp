#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

#include <lug/Graphics/Vulkan/API/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

void CommandBuffer::copyImage(const CommandBuffer::CmdCopyImage& parameters) const {
    vkCmdCopyImage(
        _commandBuffer,
        static_cast<VkImage>(parameters.srcImage),
        parameters.srcImageLayout,
        static_cast<VkImage>(parameters.dstImage),
        parameters.dstImageLayout,
        static_cast<uint32_t>(parameters.regions.size()),
        parameters.regions.data()
        );
}

} // API
} // Vulkan
} // Graphics
} // lug
