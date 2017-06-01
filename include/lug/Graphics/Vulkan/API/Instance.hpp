#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Instance;
} // Builder

class LUG_GRAPHICS_API Instance {
    friend class Builder::Instance;

public:
    Instance() = default;

    Instance(const Instance&) = delete;
    Instance(Instance&& instance);

    Instance& operator=(const Instance&) = delete;
    Instance& operator=(Instance&& instance);

    ~Instance();

    explicit operator VkInstance() const {
        return _instance;
    }

    template <typename Function>
    Function getProcAddr(const char* name) const;

    void destroy();

private:
    explicit Instance(VkInstance instance);

private:
    VkInstance _instance{VK_NULL_HANDLE};
};

#include <lug/Graphics/Vulkan/API/Instance.inl>

} // API
} // Vulkan
} // Graphics
} // lug
