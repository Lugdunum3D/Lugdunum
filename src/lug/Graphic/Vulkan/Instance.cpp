#include <lug/Graphic/Vulkan/Instance.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

Instance::Instance(VkInstance instance) : _instance(instance) {}

Instance::Instance(Instance&& instance) {
    _instance = instance._instance;
    instance._instance = VK_NULL_HANDLE;
}

Instance& Instance::operator=(Instance&& instance) {
    _instance = instance._instance;
    instance._instance = VK_NULL_HANDLE;

    return *this;
}

Instance::~Instance() {
    if (_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(_instance, nullptr);
        _instance = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphic
} // lug
