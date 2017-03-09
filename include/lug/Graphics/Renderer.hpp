#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Render/Window.hpp>

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
        std::string shadersRoot;
    };

public:
    Renderer(Graphics& graphics);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual ~Renderer() = default;

    virtual bool beginInit(const char* appName, uint32_t appVersion, const InitInfo& initInfo) = 0;
    virtual bool finishInit() = 0;

    virtual bool beginFrame() = 0;
    virtual bool endFrame() = 0;

    virtual Render::Window* createWindow(Render::Window::InitInfo& initInfo) = 0;
    virtual Render::Window* getWindow() = 0;

    const InitInfo& getInfo() const;

protected:
    Graphics& _graphics;
    InitInfo _initInfo;
};

#include <lug/Graphics/Renderer.inl>

} // Graphics
} // lug
