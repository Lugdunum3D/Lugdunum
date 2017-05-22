#pragma once

#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

class PipelineLayout {
public:
    PipelineLayout(const API::Device& device);

    PipelineLayout(const PipelineLayout&&) = delete;
    PipelineLayout(PipelineLayout&&) = delete;

    PipelineLayout& operator=(const PipelineLayout&&) = delete;
    PipelineLayout& operator=(PipelineLayout&&) = delete;

    ~PipelineLayout() = default;

    // Setters
    void setPushConstants(std::vector<VkPushConstantRange> pushConstants);
    void setDescriptorSetLayouts(std::vector<API::DescriptorSetLayout> descriptorSetLayouts);

    // Build methods
    bool build(API::PipelineLayout& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::PipelineLayout> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;
    std::vector<VkPushConstantRange> _pushConstants{};
    std::vector<API::DescriptorSetLayout> _descriptorSetLayouts{};
};

#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
