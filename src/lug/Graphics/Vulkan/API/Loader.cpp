#include <lug/Graphics/Vulkan/API/Loader.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Instance.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

bool Loader::loadCoreFunctions() {
#if defined(LUG_SYSTEM_WINDOWS)
    _handle = System::Library::open("vulkan-1.dll");
#elif defined(LUG_SYSTEM_ANDROID)
    _handle = System::Library::open("libvulkan.so");
#else
    _handle = System::Library::open("libvulkan.so.1");
#endif

    if (!_handle) {
        return false;
    }

#define LUG_LOAD_VULKAN_FUNCTIONS(name)                          \
    {                                                            \
        name = System::Library::sym<PFN_##name>(_handle, #name); \
        if (!name) {                                             \
            return false;                                        \
        }                                                        \
    }

    LUG_EXPORTED_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);

#undef LUG_LOAD_VULKAN_FUNCTIONS

#define LUG_LOAD_VULKAN_FUNCTIONS(name)                                             \
    {                                                                               \
        name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name)); \
        if (!name) {                                                                \
            LUG_LOG.error("Vulkan: Can't load symbol {}", #name);                   \
            return false;                                                           \
        }                                                                           \
    }

    LUG_CORE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);

#undef LUG_LOAD_VULKAN_FUNCTIONS

    return true;
}

bool Loader::loadInstanceFunctions(const Instance& instance) {
#define LUG_LOAD_VULKAN_FUNCTIONS(name)                           \
    {                                                             \
        name = instance.getProcAddr<PFN_##name>(#name);           \
        if (!name) {                                              \
            LUG_LOG.error("Vulkan: Can't load symbol {}", #name); \
            return false;                                         \
        }                                                         \
    }

    LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);

#undef LUG_LOAD_VULKAN_FUNCTIONS

    return true;
}

bool Loader::loadDeviceFunctions(const Device& device) {
#define LUG_LOAD_VULKAN_FUNCTIONS(name)                           \
    {                                                             \
        name = device.getProcAddr<PFN_##name>(#name);             \
        if (!name) {                                              \
            LUG_LOG.error("Vulkan: Can't load symbol {}", #name); \
            return false;                                         \
        }                                                         \
    }

    LUG_DEVICE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);

#undef LUG_LOAD_VULKAN_FUNCTIONS

    return true;
}

void Loader::unload() {
#define LUG_UNLOAD_VULKAN_FUNCTIONS(name) name = nullptr;
    LUG_EXPORTED_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    LUG_CORE_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
    LUG_DEVICE_VULKAN_FUNCTIONS(LUG_UNLOAD_VULKAN_FUNCTIONS);
#undef LUG_UNLOAD_VULKAN_FUNCTIONS

    System::Library::close(_handle);
}

} // API
} // Vulkan
} // Graphics
} // lug
