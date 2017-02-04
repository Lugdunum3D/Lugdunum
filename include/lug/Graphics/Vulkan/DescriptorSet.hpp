#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/Graphics/Vulkan/Buffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API DescriptorSet {
public:
    DescriptorSet(VkDescriptorSet descriptorSet = VK_NULL_HANDLE, const Device* device = nullptr);

    DescriptorSet(const DescriptorSet&) = delete;
    DescriptorSet(DescriptorSet&& DescriptorSet);

    DescriptorSet& operator=(const DescriptorSet&) = delete;
    DescriptorSet& operator=(DescriptorSet&& DescriptorSet);

    ~DescriptorSet();

    operator VkDescriptorSet() const {
        return _descriptorSet;
    }

    /**
     * @brief      Bind the descriptor set
     *
     * @param[in]  pipelineLayout  The pipeline layout to bind the descriptor set to
     * @param[in]  commandBuffer   The command buffer to submit the command to
     * @param[in]  setNb           The number of the set
     */
    void bind(const PipelineLayout* pipelineLayout, const CommandBuffer* commandBuffer, uint32_t setNb);

    /**
     * @brief      Update the descriptor set
     *
     * @param[in]  descriptorType  The descriptor type
     * @param[in]  dstBinding      The destination binding
     * @param[in]  buffer          The buffer to bind to the descriptor set
     * @param[in]  offset          The offset in the buffer
     * @param[in]  range           The range of the set
     */
    void update(VkDescriptorType descriptorType, uint32_t dstBinding, const Buffer* buffer, uint32_t offset, uint32_t range);

    void destroy();

private:
    VkDescriptorSet _descriptorSet{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
