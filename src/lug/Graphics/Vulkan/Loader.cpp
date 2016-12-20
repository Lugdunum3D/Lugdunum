#include <lug/Graphics/Vulkan/Loader.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

// TODO: Handle errors
Loader::Loader() {
#if defined(LUG_SYSTEM_WINDOWS)
    _handle = System::Library::open("vulkan-1.dll");
#elif defined(LUG_SYSTEM_ANDROID)
    _handle = System::Library::open("libvulkan.so");
#else
    _handle = System::Library::open("libvulkan.so.1");
#endif

    #define LUG_LOAD_VULKAN_FUNCTIONS(name) name = System::Library::sym<PFN_##name>(_handle, #name);
    LUG_EXPORTED_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);
    #undef LUG_LOAD_VULKAN_FUNCTIONS

    #define LUG_LOAD_VULKAN_FUNCTIONS(name) name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name));
    LUG_CORE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);
    #undef LUG_LOAD_VULKAN_FUNCTIONS
}

Loader::~Loader() {
    #define LUG_UNLOAD_VULKAN_FUNCTIONS(name) name = nullptr;
    LUG_EXPORTED_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    LUG_CORE_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    LUG_DEVICE_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    #undef LUG_UNLOAD_VULKAN_FUNCTIONS

    System::Library::close(_handle);
}

// TODO: Handle errors
void Loader::loadInstanceFunctions(VkInstance instance) {
    #define LUG_LOAD_VULKAN_FUNCTIONS(name) name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(instance, #name));
    LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);
    #undef LUG_LOAD_VULKAN_FUNCTIONS
}

// TODO: Handle errors
void Loader::loadDeviceFunctions(VkDevice device) {
    #define LUG_LOAD_VULKAN_FUNCTIONS(name) name = reinterpret_cast<PFN_##name>(vkGetDeviceProcAddr(device, #name));
    LUG_DEVICE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);
    #undef LUG_LOAD_VULKAN_FUNCTIONS
}

} // Vulkan
} // Graphics
} // lug
