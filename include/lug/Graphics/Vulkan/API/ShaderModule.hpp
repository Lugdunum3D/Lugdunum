#pragma once

#include <array>
#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {
class ShaderModule;
}

class LUG_GRAPHICS_API ShaderModule {
    friend Builder::ShaderModule;

public:
    ShaderModule() = default;

    ShaderModule(const ShaderModule&) = delete;
    ShaderModule(ShaderModule&& shaderModule);

    ShaderModule& operator=(const ShaderModule&) = delete;
    ShaderModule& operator=(ShaderModule&& shaderModule);

    ~ShaderModule();

    explicit operator VkShaderModule() const {
        return _shaderModule;
    }

    void destroy();

private:
    explicit ShaderModule(VkShaderModule shaderModule, const Device* device);

private:
    VkShaderModule _shaderModule{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
