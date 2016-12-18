#pragma once

#include <algorithm>
#include <memory>
#include <set>
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

    void addModule(Module::Type type);
    void addOptionnalModule(Module::Type type);

    void removeModule(Module::Type type);
    void removeOptionnalModule(Module::Type type);

    const std::set<Module::Type>& getMandatoryModules() const;
    const std::set<Module::Type>& getOptionnalModules() const;

    bool isModuleLoaded(Module::Type type) const;
    const std::set<Module::Type>& getLoadedModules() const;

    // TODO: setRendererType, getRendererType
    // TODO: getRenderer

    const Core::Application::Info& getAppInfo() const;

private:
    Core::Application::Info _appInfo;

    std::set<Module::Type> _mandatoryModules{Module::Type::Core};
    std::set<Module::Type> _optionnalModules{};
    std::set<Module::Type> _loadedModules{};

    Renderer::Type _rendererType{Renderer::Type::Vulkan};
    std::unique_ptr<Renderer> _renderer{nullptr};
};

#include <lug/Graphic/Graphic.inl>

} // Graphic
} // lug
