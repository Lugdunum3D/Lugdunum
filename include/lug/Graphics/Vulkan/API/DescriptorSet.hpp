#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class DescriptorSet;
} // Builder

class Buffer;
class Device;

class LUG_GRAPHICS_API DescriptorSet {
    friend class Builder::DescriptorSet;

public:
    DescriptorSet() = default;

    DescriptorSet(const DescriptorSet&) = delete;
    DescriptorSet(DescriptorSet&& DescriptorSet);

    DescriptorSet& operator=(const DescriptorSet&) = delete;
    DescriptorSet& operator=(DescriptorSet&& DescriptorSet);

    ~DescriptorSet();

    explicit operator VkDescriptorSet() const {
        return _descriptorSet;
    }

    /**
     * @brief      Update the descriptor set buffers
     *
     * @param[in]  dstBinding      The destination binding
     * @param[in]  dstArrayElement The starting element in the descriptor set binding array
     * @param[in]  descriptorType  The descriptor type
     * @param[in]  bufferInfos     The buffers to bind to the descriptor set
     */
    void updateBuffers(
        uint32_t dstBinding,
        uint32_t dstArrayElement,
        VkDescriptorType descriptorType,
        const std::vector<VkDescriptorBufferInfo>& bufferInfos
    ) const;

    /**
     * @brief      Update the descriptor set images
     *
     * @param[in]  dstBinding      The destination binding
     * @param[in]  dstArrayElement The starting element in the descriptor set binding array
     * @param[in]  descriptorType  The descriptor type
     * @param[in]  imageInfos     The images to bind to the descriptor set
     */
    void updateImages(
        uint32_t dstBinding,
        uint32_t dstArrayElement,
        VkDescriptorType descriptorType,
        const std::vector<VkDescriptorImageInfo>& imageInfos
    ) const;

    void destroy();

private:
    explicit DescriptorSet(VkDescriptorSet descriptorSet, const Device* device);

private:
    VkDescriptorSet _descriptorSet{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
