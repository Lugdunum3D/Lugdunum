#pragma once

#include <lug/Graphics/Vulkan/API/Instance.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Surface;
} // Builder

class Surface {
    friend class Builder::Surface;

public:
    Surface() = default;

    Surface(const Surface&) = delete;
    Surface(Surface&& surface);

    Surface& operator=(const Surface&) = delete;
    Surface& operator=(Surface&& surface);

    ~Surface();

    explicit operator VkSurfaceKHR() const {
        return _surface;
    }

    void destroy();

private:
    explicit Surface(VkSurfaceKHR surface, const Instance* instance);

private:
    VkSurfaceKHR _surface{VK_NULL_HANDLE};
    const Instance* _instance{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
