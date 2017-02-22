#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/System/Library.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Instance;
class Device;

class LUG_GRAPHICS_API Loader {
public:
    Loader() = default;

    Loader(const Loader&) = delete;
    Loader(Loader&&) = delete;

    Loader& operator=(const Loader&) = delete;
    Loader& operator=(Loader&&) = delete;

    ~Loader() = default;

    bool loadCoreFunctions();
    bool loadInstanceFunctions(const Instance& instance);
    bool loadDeviceFunctions(const Device& device);

    void unload();

private:
    System::Library::Handle _handle{nullptr};
};

} // Vulkan
} // Graphics
} // lug
