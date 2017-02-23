#include <lug/Graphics/Vulkan/ShaderModule.hpp>
#include <fstream>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

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
        vkDestroyShaderModule(*_device, _shaderModule, nullptr);
        _shaderModule = VK_NULL_HANDLE;
    }
}

std::unique_ptr<ShaderModule> ShaderModule::create(const std::string& file, const Device* device) {
    // TODO: replace opening file with something more global
    std::ifstream shaderCode(file, std::ios::binary);

    if (!shaderCode.good()) {
        // TODO: use errno to print the correct error
        LUG_LOG.error("RendererVulkan: Can't open file \"{}\"", file);
        return nullptr;
    }

    shaderCode.seekg(0, shaderCode.end);
    uint32_t shaderCodeSize = static_cast<uint32_t>(shaderCode.tellg());
    shaderCode.seekg(0, shaderCode.beg);

    char* buffer = new char[shaderCodeSize];

    shaderCode.read(buffer, shaderCodeSize);
    shaderCode.close();

    VkShaderModuleCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.codeSize = shaderCodeSize,
        createInfo.pCode = reinterpret_cast<uint32_t*>(buffer),
    };

    VkShaderModule shaderModule = VK_NULL_HANDLE;
    VkResult result = vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create shader module \"{}\": {}", file, result);
        return nullptr;
    }

    delete[] buffer;

    return std::unique_ptr<ShaderModule>(new ShaderModule(shaderModule, device));
}

} // Vulkan
} // Graphics
} // lug
