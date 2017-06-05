#include <imgui.h>

#include <lug\Graphics\Vulkan\Gui.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/CommandPool.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Gui::Gui(Renderer & renderer, Render::Window & window) : _renderer(renderer), _window(window) {
}

Gui::~Gui() {
    //vkDestroySampler(static_cast<VkDevice>(_renderer.getDevice()), _sampler, nullptr);
}

bool Gui::init(const std::vector<std::unique_ptr<API::ImageView>>& imageViews) {
    ImGuiIO& io = ImGui::GetIO();

    initKeyMapping();

    io.DisplaySize = ImVec2(_window.getWidth(), _window.getHeight());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    // Get present queue family and retrieve the first queue
    {
        _presentQueueFamily = _renderer.getDevice().getQueueFamily(VK_QUEUE_GRAPHICS_BIT);
        if (!_presentQueueFamily) {
            LUG_LOG.error("Gui::init: Can't find VK_QUEUE_GRAPHICS_BIT queue family");
            return false;
        }
        _presentQueue = &_presentQueueFamily->getQueues().front();
        if (!_presentQueue) {
            LUG_LOG.error("Gui::init: Can't find presentation queue");
            return false;
        }
    }

    // Create command pool of present queue
    {
        VkResult result{ VK_SUCCESS };
        API::Builder::CommandPool commandPoolBuilder(_renderer.getDevice(), *_presentQueueFamily);
        if (!commandPoolBuilder.build(_commandPool, &result)) {
            LUG_LOG.error("Gui::init: Can't create a command pool: {}", result);
            return false;
        }
    }
    API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _commandPool);

    // Command buffers
    {
        VkResult result{ VK_SUCCESS };
        if (!commandBufferBuilder.build(_commandBuffers, &result)) {
            LUG_LOG.error("Gui::init: Can't create the command buffer: {}", result);
            return false;
        }
    }

    _indexBuffers.resize(imageViews.size());
    _vertexBuffers.resize(imageViews.size());
    _vertexDeviceMemories.resize(imageViews.size());
    _indexDeviceMemories.resize(imageViews.size());
    _vertexCounts.resize(imageViews.size()); // NOTE: These might be unecessary as we might be able to retrieve their size 
    _indexCounts.resize(imageViews.size()); // NOTE: "  "

    return initFontsTexture() && initPipeline() && initFramebuffers(imageViews);
}

void Gui::initKeyMapping() {
    ImGuiIO& io = ImGui::GetIO();

    // Keyboard mapping for specific shortcuts used by ImGui
    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(lug::Window::Keyboard::Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(lug::Window::Keyboard::Key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(lug::Window::Keyboard::Key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(lug::Window::Keyboard::Key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(lug::Window::Keyboard::Key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(lug::Window::Keyboard::Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(lug::Window::Keyboard::Key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(lug::Window::Keyboard::Key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(lug::Window::Keyboard::Key::End);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(lug::Window::Keyboard::Key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(lug::Window::Keyboard::Key::BackSpace);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(lug::Window::Keyboard::Key::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(lug::Window::Keyboard::Key::Escape);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(lug::Window::Keyboard::Key::A);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(lug::Window::Keyboard::Key::C);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(lug::Window::Keyboard::Key::V);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(lug::Window::Keyboard::Key::X);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(lug::Window::Keyboard::Key::Y);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(lug::Window::Keyboard::Key::Z);
}

bool Gui::initFontsTexture() {
    return false;
}

bool Gui::initFramebuffers(const std::vector<std::unique_ptr<API::ImageView>>& /*imageViews*/) {
    return false;
}

bool Gui::initPipeline() {
    return false;
}

} // Vulkan
} // Graphics
} // lug