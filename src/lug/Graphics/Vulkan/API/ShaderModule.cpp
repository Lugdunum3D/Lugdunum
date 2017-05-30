#include <lug/Graphics/Vulkan/API/ShaderModule.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

ShaderModule::ShaderModule(VkShaderModule shaderModule, const Device* device) : _shaderModule(shaderModule), _device(device) {}

ShaderModule::ShaderModule(ShaderModule&& shaderModule) {
    _shaderModule = shaderModule._shaderModule;
    _device = shaderModule._device;
    shaderModule._shaderModule = VK_NULL_HANDLE;
    shaderModule._device = nullptr;
}

ShaderModule& ShaderModule::operator=(ShaderModule&& shaderModule) {
    destroy();

    _shaderModule = shaderModule._shaderModule;
    _device = shaderModule._device;
    shaderModule._shaderModule = VK_NULL_HANDLE;
    shaderModule._device = nullptr;

    return *this;
}

ShaderModule::~ShaderModule() {
    destroy();
}

void ShaderModule::destroy() {
    if (_shaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(static_cast<VkDevice>(*_device), _shaderModule, nullptr);
        _shaderModule = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
