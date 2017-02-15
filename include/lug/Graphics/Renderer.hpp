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

    struct InitInfo {
        std::set<Module::Type> mandatoryModules;
        std::set<Module::Type> optionalModules;
        bool useDiscreteGPU;
    };

public:
    Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    virtual ~Renderer() = default;

    virtual std::set<Module::Type> init(const char* appName, uint32_t appVersion, const InitInfo& initInfo) = 0;

    virtual bool beginFrame() = 0;
    virtual bool endFrame() = 0;

    virtual RenderWindow* createWindow(RenderWindow::InitInfo& initInfo) = 0;
    virtual RenderWindow* getWindow() = 0;
};

} // Graphics
} // lug
