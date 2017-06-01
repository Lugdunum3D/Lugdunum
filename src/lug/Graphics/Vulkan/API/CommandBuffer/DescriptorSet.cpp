#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>

#include <algorithm>

#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

void CommandBuffer::bindDescriptorSets(const CommandBuffer::CmdBindDescriptors& parameters) const {
    std::vector<VkDescriptorSet> descriptorSets;

    std::transform(
        parameters.descriptorSets.cbegin(),
        parameters.descriptorSets.cend(),

        std::back_inserter(descriptorSets),

        [](const API::DescriptorSet* descriptorSet) {
            return static_cast<VkDescriptorSet>(*descriptorSet);
        }
    );

    vkCmdBindDescriptorSets(
        _commandBuffer,
        parameters.pipelineBindPoint,
        static_cast<VkPipelineLayout>(parameters.pipelineLayout),
        parameters.firstSet,
        static_cast<uint32_t>(parameters.descriptorSets.size()),
        descriptorSets.data(),
        static_cast<uint32_t>(parameters.dynamicOffsets.size()),
        parameters.dynamicOffsets.data()
    );
}

} // API
} // Vulkan
} // Graphics
} // lug
