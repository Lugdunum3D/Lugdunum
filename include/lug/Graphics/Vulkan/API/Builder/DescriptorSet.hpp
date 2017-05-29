#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class DescriptorPool;
class Device;

namespace Builder {

class DescriptorSet {
public:
    DescriptorSet(const API::Device& device, const API::DescriptorPool& descriptorPool);

    DescriptorSet(const DescriptorSet&&) = delete;
    DescriptorSet(DescriptorSet&&) = delete;

    DescriptorSet& operator=(const DescriptorSet&&) = delete;
    DescriptorSet& operator=(DescriptorSet&&) = delete;

    ~DescriptorSet() = default;

    // Setters
    void setDescriptorSetLayouts(const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);

    // Build methods
    bool build(API::DescriptorSet& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::DescriptorSet> build(VkResult* returnResult = nullptr);

    bool build(std::vector<API::DescriptorSet>& commandBuffers, VkResult* returnResult = nullptr);
    std::vector<std::unique_ptr<API::DescriptorSet>> build(uint32_t count, VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    const API::DescriptorPool& _descriptorPool;
    std::vector<VkDescriptorSetLayout> _descriptorSetLayouts{};
    std::vector<VkDescriptorSet> _poolSizes{};
};

#include <lug/Graphics/Vulkan/API/Builder/DescriptorSet.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
