#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Gui {
public:
    // UI params are set via push constants
    struct PushConstBlock {
        lug::Math::Vec2f scale;
        lug::Math::Vec2f translate;
    } pushConstBlock;
public:
    Gui() = delete;

    Gui(Renderer& renderer, Render::Window &window);

    Gui(const Gui&) = delete;

    Gui& operator=(const Gui&) = delete;
    Gui& operator=(Gui&&) = delete;

    ~Gui();

    bool init(const std::vector<std::unique_ptr<API::ImageView>>& imageViews);
    void initKeyMapping();
    bool initFontsTexture();
    bool initFramebuffers(const std::vector<std::unique_ptr<API::ImageView>>& imageViews);
    bool initPipeline();


private:
    Renderer& _renderer;
    Render::Window& _window;

    const API::Queue* _presentQueue{ nullptr };
    const API::QueueFamily* _presentQueueFamily{ nullptr };
    const API::Queue* _transferQueue{ nullptr };
    const API::QueueFamily* _transferQueueFamily{ nullptr };

    API::CommandPool _commandPool{};
    std::vector<Vulkan::API::CommandBuffer> _commandBuffers;

    std::vector<std::unique_ptr<Vulkan::API::DeviceMemory>> _vertexDeviceMemories;
    std::vector<std::unique_ptr<Vulkan::API::DeviceMemory>> _indexDeviceMemories;

    std::vector<std::unique_ptr<Vulkan::API::Buffer>> _indexBuffers;
    std::vector<std::unique_ptr<Vulkan::API::Buffer>> _vertexBuffers;

    std::vector<int> _vertexCounts;
    std::vector<int> _indexCounts;

    std::unique_ptr<API::Image> _image;
    std::unique_ptr<API::ImageView> _imageView;

    API::Buffer _stagingBuffer;

};

} // Vulkan
} // Graphics
} // lug
