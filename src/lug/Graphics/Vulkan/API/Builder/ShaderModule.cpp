#include <lug/Graphics/Vulkan/API/Builder/ShaderModule.hpp>

#include <fstream>

#if defined(LUG_SYSTEM_ANDROID)
    #include <android/asset_manager.h>

    #include <lug/Window/Android/WindowImplAndroid.hpp>
    #include <lug/Window/Window.hpp>
#endif

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

ShaderModule::ShaderModule(const API::Device& device) : _device{device} {}

bool ShaderModule::loadFromFile(const std::string& filename) {
#if defined(LUG_SYSTEM_ANDROID)
    // Load shader from compressed asset
    AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, filename.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", filename);
        return false;
    }

    _codeSize = AAsset_getLength(asset);

    if (_codeSize <= 0) {
        LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", filename);
        return false;
    }

    _data.resize(_codeSize % 4 ? (_codeSize + 4 - _codeSize % 4) : (_codeSize));

    AAsset_read(asset, reinterpret_cast<char*>(_data.data()), _codeSize);
    AAsset_close(asset);
#else
    std::ifstream shaderFile(filename, std::ios::binary);

    if (!shaderFile.good()) {
        LUG_LOG.error("Builder::ShaderModule: Can't open file \"{}\"", filename);
        return false;
    }

    shaderFile.seekg(0, shaderFile.end);
    _codeSize = shaderFile.tellg();
    shaderFile.seekg(0, shaderFile.beg);

    _data.resize(_codeSize % 4 ? (_codeSize + 4 - _codeSize % 4) : (_codeSize));

    shaderFile.read(reinterpret_cast<char*>(_data.data()), _codeSize);
    shaderFile.close();
#endif

    return true;
}

void ShaderModule::loadFromData(const std::vector<uint32_t>& data) {
    _codeSize = data.size() * sizeof(uint32_t);
    _data = data;
}

bool ShaderModule::build(API::ShaderModule& shaderModule, VkResult* returnResult) {
    const VkShaderModuleCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.codeSize */ _codeSize,
        /* createInfo.pCode */ _data.data()
    };

    VkShaderModule vkShaderModule = VK_NULL_HANDLE;
    VkResult result = vkCreateShaderModule(static_cast<VkDevice>(_device), &createInfo, nullptr, &vkShaderModule);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    shaderModule = API::ShaderModule(vkShaderModule, &_device);

    return true;
}

std::unique_ptr<API::ShaderModule> ShaderModule::build(VkResult* returnResult) {
    std::unique_ptr<API::ShaderModule> shaderModule = std::make_unique<API::ShaderModule>();
    return build(*shaderModule, returnResult) ? std::move(shaderModule) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
