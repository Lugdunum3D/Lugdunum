#pragma once

#include <cstdint>
#include <set>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/RenderWindow.hpp>

namespace lug {
namespace Graphics {

class Graphics;

class LUG_GRAPHICS_API Renderer {
public:
    enum class Type : uint8_t {
        Vulkan
    };

    struct InitInfo {
        bool useDiscreteGPU;
    };

public:
    Renderer(Graphics& graphics);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual ~Renderer() = default;

    virtual bool beginInit(const char* appName, uint32_t appVersion, const InitInfo& initInfo) = 0;
    virtual bool finishInit(const InitInfo& initInfo) = 0;

    virtual bool beginFrame() = 0;
    virtual bool endFrame() = 0;

    virtual RenderWindow* createWindow(RenderWindow::InitInfo& initInfo) = 0;
    virtual RenderWindow* getWindow() = 0;

protected:
    Graphics& _graphics;
};

} // Graphics
} // lug
