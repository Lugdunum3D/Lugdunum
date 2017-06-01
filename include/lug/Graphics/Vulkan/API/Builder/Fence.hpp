#pragma once

#include <memory>

#include <lug/Graphics/Vulkan/API/Fence.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class Fence {
public:
    Fence(const API::Device& device);

    Fence(const Fence&&) = delete;
    Fence(Fence&&) = delete;

    Fence& operator=(const Fence&&) = delete;
    Fence& operator=(Fence&&) = delete;

    ~Fence() = default;

    // Setters
    void setFlags(VkFenceCreateFlags flags);

    // Build methods
    bool build(API::Fence& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Fence> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    VkFenceCreateFlags _flags{0};
};

#include <lug/Graphics/Vulkan/API/Builder/Fence.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
