#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Instance {
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

    template <typename Function>
    Function getProcAddr(const char* name) const;

    void destroy();

private:
    VkInstance _instance{VK_NULL_HANDLE};
};

#include <lug/Graphics/Vulkan/Instance.inl>

} // Vulkan
} // Graphics
} // lug
