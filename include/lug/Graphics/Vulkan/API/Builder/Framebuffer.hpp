#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;
class ImageView;
class RenderPass;

namespace Builder {

class Framebuffer {
public:
    Framebuffer(const API::Device& device);

    Framebuffer(const Framebuffer&&) = delete;
    Framebuffer(Framebuffer&&) = delete;

    Framebuffer& operator=(const Framebuffer&&) = delete;
    Framebuffer& operator=(Framebuffer&&) = delete;

    ~Framebuffer() = default;

    // Setters
    void setRenderPass(const API::RenderPass* renderPass);
    void addAttachment(const API::ImageView* attachment);
    void setWidth(uint32_t width);
    void setHeight(uint32_t height);
    void setLayers(uint32_t layers);

    // Build methods
    bool build(API::Framebuffer& framebuffer, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Framebuffer> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    const API::RenderPass* _renderPass{nullptr};
    std::vector<const API::ImageView*> _attachments{};
    uint32_t _width{0};
    uint32_t _height{0};
    uint32_t _layers{1};
};

#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
