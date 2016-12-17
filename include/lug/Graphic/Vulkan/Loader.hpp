#pragma once

#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>
#include <lug/System/Library.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

class LUG_GRAPHIC_API Loader {
public:
    Loader();

    Loader(const Loader&) = delete;
    Loader(Loader&&) = delete;

    Loader& operator=(const Loader&) = delete;
    Loader& operator=(Loader&&) = delete;

    ~Loader();

    void loadInstanceFunctions(VkInstance instance);

private:
    System::Library::Handle _handle{nullptr};
};

} // Vulkan
} // Graphic
} // lug
