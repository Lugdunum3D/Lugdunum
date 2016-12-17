#pragma once

#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

class LUG_GRAPHIC_API Instance {
public:
    explicit Instance(VkInstance instance = VK_NULL_HANDLE);

    Instance(const Instance&) = delete;
    Instance(Instance&& instance);

    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&& instance);

    ~Instance();

    operator VkInstance() const {
        return _instance;
    }

private:
    VkInstance _instance{VK_NULL_HANDLE};
};

} // Vulkan
} // Graphic
} // lug
