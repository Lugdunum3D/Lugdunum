#include <lug/Graphic/Vulkan/Loader.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>
#include <lug/System/Library.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {
namespace Loader {

void loadCore() {
#if defined(LUG_SYSTEM_WINDOWS)
    System::Library::Handle handle = System::Library::open("vulkan-1.dll");
#else
    System::Library::Handle handle = System::Library::open("libvulkan.so.1");
#endif

    #define LUG_LOAD_VULKAN_FUNCTIONS(name) name = System::Library::sym<PFN_##name>(handle, #name);
    LUG_CORE_VULKAN_FUNCTIONS(LUG_LOAD_VULKAN_FUNCTIONS);
    #undef LUG_LOAD_VULKAN_FUNCTIONS

    System::Library::close(handle);
}

}
}
}
}
