#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/System/Library.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Loader {
public:
    Loader();

    Loader(const Loader&) = delete;
    Loader(Loader&&) = delete;

    Loader& operator=(const Loader&) = delete;
    Loader& operator=(Loader&&) = delete;

    ~Loader();

    void loadInstanceFunctions(VkInstance instance);
    void loadDeviceFunctions(VkDevice device);

private:
    System::Library::Handle _handle{nullptr};
};

} // Vulkan
} // Graphics
} // lug
