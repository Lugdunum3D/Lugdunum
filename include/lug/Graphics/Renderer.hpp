#pragma once

#include <cstdint>
#include <set>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/RenderWindow.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Renderer {
public:
    enum class Type : uint8_t {
        Vulkan
    };

public:
    Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual ~Renderer() = default;

    virtual std::set<Module::Type> init() = 0;

    virtual std::unique_ptr<RenderWindow> createWindow(uint16_t width, uint16_t height, const std::string& title, lug::Window::Style style) = 0;
};

} // Graphics
} // lug
