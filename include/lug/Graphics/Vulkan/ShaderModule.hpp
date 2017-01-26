#pragma once

#include <array>
#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API ShaderModule {
public:
    explicit ShaderModule(VkShaderModule shaderModule = VK_NULL_HANDLE, const Device* device = nullptr);

    ShaderModule(const ShaderModule&) = delete;
    ShaderModule(ShaderModule&& shaderModule);

    ShaderModule& operator=(const ShaderModule&) = delete;
    ShaderModule& operator=(ShaderModule&& shaderModule);

    ~ShaderModule();

    operator VkShaderModule() const {
        return _shaderModule;
    }

    void destroy();

    static std::unique_ptr<ShaderModule> create(const std::string& file, const Device* device);

private:
    VkShaderModule _shaderModule{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
