#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/ShaderModule.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class ShaderModule {
public:
    ShaderModule(const API::Device& device);

    ShaderModule(const ShaderModule&&) = delete;
    ShaderModule(ShaderModule&&) = delete;

    ShaderModule& operator=(const ShaderModule&&) = delete;
    ShaderModule& operator=(ShaderModule&&) = delete;

    ~ShaderModule() = default;

    bool loadFromFile(const std::string& filename);
    void loadFromData(const std::vector<uint32_t>& data);

    // Build methods
    bool build(API::ShaderModule& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::ShaderModule> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    std::vector<uint32_t> _data;
};

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
