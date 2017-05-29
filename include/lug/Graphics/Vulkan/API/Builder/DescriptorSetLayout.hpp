#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/DescriptorSetLayout.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class DescriptorSetLayout {
public:
    DescriptorSetLayout(const API::Device& device);

    DescriptorSetLayout(const DescriptorSetLayout&&) = delete;
    DescriptorSetLayout(DescriptorSetLayout&&) = delete;

    DescriptorSetLayout& operator=(const DescriptorSetLayout&&) = delete;
    DescriptorSetLayout& operator=(DescriptorSetLayout&&) = delete;

    ~DescriptorSetLayout() = default;

    // Setters
    void setBindings(const std::vector<VkDescriptorSetLayoutBinding>& bindings);

    // Build methods
    bool build(API::DescriptorSetLayout& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::DescriptorSetLayout> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    std::vector<VkDescriptorSetLayoutBinding> _bindings{};
};

#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
