#pragma once

#include <memory>
#include <vector>
#include <lug/Core/Application.hpp>
#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Module.hpp>
#include <lug/Graphic/Renderer.hpp>

namespace lug {
namespace Graphic {

class LUG_GRAPHIC_API Graphic {
public:
    explicit Graphic(const Core::Application::Info& appInfo);

    Graphic(const Graphic&) = delete;
    Graphic(Graphic&&) = delete;

    Graphic& operator=(const Graphic&) = delete;
    Graphic& operator=(Graphic&&) = delete;

    ~Graphic() = default;

    void init();

    // TODO: optionnal modules
    // TODO: addModule, removeModule, isModuleLoaded
    constexpr const std::vector<Module::Type>& getModulesLoaded() const;

    // TODO: setRendererType, getRendererType
    // TODO: getRenderer

    constexpr const Core::Application::Info& getAppInfo() const;

private:
    Core::Application::Info _appInfo;

    std::vector<Module::Type> _modules{Module::Type::Core};

    Renderer::Type _rendererType{Renderer::Type::Vulkan};
    std::unique_ptr<Renderer> _renderer{nullptr};
};

#include <lug/Graphic/Graphic.inl>

} // Graphic
} // lug
